/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015, Peter Buelow
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
**/
#include "application.h"
#include "FastLED-Sparkcore/firmware/FastLED.h"
#include "sunset/firmware/SunSet.h"
#include "WindowLights.h"

#include "Christmas.h"
#include "Halloween.h"
#include "Valentines.h"
#include "IndependenceDay.h"
#include "Thanksgiving.h"
#include "SunPosition.h"
#include "Norah.h"
#include "Maddie.h"
#include "MeteorShower.h"
#include "Twinkles.h"

SYSTEM_MODE(AUTOMATIC);

using namespace NSFastLED;

CRGB strip[NUM_STRIPS][LEDS_PER_STRIP];
SunSet sun;
bool defaultProg;
bool runAnyway;
int lastMinute;
int activeProgram;
bool running;

const TProgmemRGBPalette16 Christmas_p =
{
		CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::Orange,
		CRGB::Pink, CRGB::Yellow, CRGB::White, CRGB::Blue,
		CRGB::Pink, CRGB::Blue, CRGB::Green, CRGB::Orange,
		CRGB::Yellow, CRGB::Red, CRGB::White, CRGB::Red
};

const TProgmemRGBPalette16 Snow_p =
{  0x404040, 0x404040, 0x404040, 0x404040,
   0x404040, 0x404040, 0x404040, 0x404040,
   0x404040, 0x404040, 0x404040, 0x404040,
   0x404040, 0x404040, 0x404040, 0xFFFFFF
};

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemRGBPalette16 FairyLight_p =
{  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight,
   HALFFAIRY,        HALFFAIRY,        CRGB::FairyLight, CRGB::FairyLight,
   QUARTERFAIRY,     QUARTERFAIRY,     CRGB::FairyLight, CRGB::FairyLight,
   CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight
};

Christmas wink(NUM_LEDS, NUM_ACTIVE);
Twinkles twink(Christmas_p);
Twinkles snow(Snow_p);
Valentines vday(NUM_LEDS);
Independence iday(NUM_LEDS);
Halloween hday(NUM_LEDS);
Thanksgiving tday(NUM_LEDS);
Norah nbday(NUM_LEDS);
Maddie mbday(NUM_LEDS);
MeteorShower meteorStrip1;
MeteorShower meteorStrip2;
MeteorShower meteorStrip3;
MeteorShower meteorStrip4;

const uint8_t _usDSTStart[22] = { 8,13,12,11,10, 8,14,13,12,10, 9, 8,14,12,11,10, 9,14,13,12,11, 9};
const uint8_t _usDSTEnd[22]   = { 1, 6, 5, 4, 3, 1, 7, 6, 5, 3, 2, 1, 7, 5, 4, 3, 2, 7, 6, 5, 4, 2};

int currentTimeZone()
{
    if (Time.month() > 3 && Time.month() < 11) {
        return DST_OFFSET;
    }
    if (Time.month() == 3) {
        if ((Time.day() == _usDSTStart[Time.year() -  TIME_BASE_YEAR]) && Time.hour() >= 2)
            return DST_OFFSET;
        if (Time.day() > _usDSTStart[Time.year() -  TIME_BASE_YEAR])
            return DST_OFFSET;
    }
    if (Time.month() == 11) {
        if ((Time.day() == _usDSTEnd[Time.year() -  TIME_BASE_YEAR]) && Time.hour() <=2)
            return DST_OFFSET;
        if (Time.day() > _usDSTEnd[Time.year() -  TIME_BASE_YEAR])
            return CST_OFFSET;
    }

    return CST_OFFSET;
}

bool validNightRunTime()
{
	double sunset = sun.calcSunset();
	double minsPastMidnight = Time.hour() * 60 + Time.minute();

	if (runAnyway)
		return true;

	if (minsPastMidnight >= (sunset - 30) && (minsPastMidnight < 1380)) {
		return true;
	}

	return false;
}

bool validFullDayRunTime()
{
	double sunrise = sun.calcSunrise();
	double sunset = sun.calcSunset();
	double minsPastMidnight = Time.hour() * 60 + Time.minute();

	if (runAnyway)
		return true;

	if ((minsPastMidnight >= 240) && (minsPastMidnight < (sunrise + 15))) {
		return true;
	}
	if (minsPastMidnight >= (sunset - 30) && (minsPastMidnight < 1380)) {
		return true;
	}

	return false;
}

void pixelShutdown()
{
	for (int i = 0; i < NUM_STRIPS; i++) {
		for (int j = 0; j < LEDS_PER_STRIP; j++) {
			strip[i][j] = CRGB::Black;
		}
	}
	FastLED.show();
}

void runSnow()
{
	snow.action();
}

void runChristmas()
{
	if (!Time.day() % 2) {
		if (validFullDayRunTime() && !running) {
			running = true;
			wink.startup();
			wink.setFirstActive(20);
			wink.seeTheRainbow();
		}
		if (validFullDayRunTime() && running) {
			wink.action();
			if (random(0, 3) == 2)
				wink.addOne();
			delay(25);
		}
		if (!validFullDayRunTime() && running) {
			pixelShutdown();
			running = false;
		}
	}
	else {
		if (validFullDayRunTime() && !running) {
			running = true;
			twink.startup();
			twink.seeTheRainbow();
		}
		if (validFullDayRunTime() && running) {
			twink.action();
		}
		if (!validFullDayRunTime() && running) {
			pixelShutdown();
			running = false;
		}
	}
}

void runValentines()
{
	if (validFullDayRunTime() && !running) {
		running = true;
		vday.startup();
	}
	if (validFullDayRunTime() && running) {
		vday.action();
		delay(500);
	}
	if (!validFullDayRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runIndependence()
{
	if (validNightRunTime() && !running) {
		running = true;
		iday.startup();
	}
	if (validNightRunTime() && running) {
		iday.action();
		delay(500);
	}
	if (!validNightRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runHalloween()
{
	if (validNightRunTime() && !running) {
		running = true;
		hday.startup();
	}
	if (validNightRunTime() && running) {
		hday.action();
		if (random(0, 50) == 23) {
			hday.lightning(random(0, NUM_STRIPS));
		}
		delay(1000);
	}
	if (!validNightRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runThanksgiving()
{
	if (validNightRunTime() && !running) {
		running = true;
		tday.startup();
	}
	if (validNightRunTime() && running) {
		tday.action();
		delay(1000);
	}
	if (!validNightRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runNorah()
{
	if (validNightRunTime() && !running) {
		running = true;
		nbday.startup();
	}
	if (validNightRunTime() && running) {
		nbday.action();
		delay(1000);
	}
	if (!validNightRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runMaddie()
{
	if (validNightRunTime() && !running) {
		running = true;
		mbday.startup();
	}
	if (validNightRunTime() && running) {
		mbday.action();
		delay(1000);
	}
	if (!validNightRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runMeteorShower()
{
	if (validFullDayRunTime() && !running) {
		running = true;
		mbday.startup();
	}
	if (validFullDayRunTime() && running) {
		meteorStrip1.action();
		meteorStrip2.action();
		meteorStrip3.action();
		meteorStrip4.action();
		FastLED.show();
	}
	if (!validFullDayRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runDefault()
{
	runIndependence();
}

void programOnDeck()
{
	if (activeProgram != NO_PROGRAM)
		return;

	/* Christmas lights start on the 1st of December, and go through the end of the month */
	if (Time.month() == 12) {
		activeProgram = CHRISTMAS;
	}
	/* Valentines day lights only on the 14th */
	else if ((Time.month() == 2) && (Time.day() == 14)) {
		activeProgram = VALENTINES;
	}
	/* Independence day lights on the 4th, but also on Labor and Memorial days */
	else if ((Time.month() == 7) && (Time.day() == 4)) {
		activeProgram = INDEPENDENCE;
	}
	else if (Time.month() == 5) {
		if ((Time.day() > 24) && (Time.weekday() == 2)) {
			activeProgram = INDEPENDENCE;
		}
	}
	else if (Time.month() == 9) {
		if ((Time.day() < 8) && (Time.weekday() == 2)) {
			activeProgram = INDEPENDENCE;
		}
	}
	/* Halloween starts on the 24th and runs through the end of the month */
	else if ((Time.month() == 10) && (Time.day() > 24)) {
		activeProgram = HALLOWEEN;
	}
	/* Start turkey day lights on the 20th and run through the end of the month */
	else if (Time.month() == 11 && Time.day() > 20) {
		activeProgram = THANKSGIVING;
	}
	/* Random other days to show a pattern */
	else if (Time.month() == 4) {
		if (Time.day() == 14)
			activeProgram = NORAH_BDAY;
	}
	else if (Time.month() == 9 && Time.day() == 17) {
		activeProgram = MADDIE_BDAY;
	}
	else
		activeProgram = NO_PROGRAM;
}

void printHeartbeat()
{
    if (lastMinute == 59 && Time.minute() >= 0) {
        Particle.publish("Heartbeat", String("System Version: " + System.version() + ", Program Version: " + APP_VERSION));
        Particle.publish("Active Program", String("Active Program: " + activeProgram));
        lastMinute = Time.minute();
    }

    if (Time.minute() >= lastMinute + 1) {
        Particle.publish("Heartbeat", String("System Version: " + System.version() + ", Program Version: " + APP_VERSION));
        lastMinute = Time.minute();
    }
}

int setProgram(String prog)
{
	runAnyway = true;
	if (prog.equalsIgnoreCase("christmas")) {
		activeProgram = CHRISTMAS;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("newyears")) {
		activeProgram = NEWYEARS;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("halloween")) {
		activeProgram = HALLOWEEN;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("thanksgiving")) {
		activeProgram = THANKSGIVING;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("independence")) {
		activeProgram = INDEPENDENCE;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("maddie")) {
		activeProgram = MADDIE_BDAY;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("norah")) {
		activeProgram = NORAH_BDAY;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("meteors")) {
		activeProgram = METEOR_SHOWER;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("valentines")) {
		activeProgram = VALENTINES;
		return activeProgram;
	}
	if (prog.equalsIgnoreCase("snow")) {
		activeProgram = SNOW;
		return activeProgram;
	}
	pixelShutdown();
	runAnyway = false;
	running = false;
	activeProgram = NO_PROGRAM;
	return NO_PROGRAM;
}

void setup()
{
	Serial.begin(115200);
	pinMode(D0, OUTPUT);
	pinMode(D1, OUTPUT);
	pinMode(D2, OUTPUT);
	pinMode(D3, OUTPUT);

	delay(3000);
	FastLED.addLeds<NEOPIXEL, D3>(strip[0], NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, D2>(strip[1], NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, D1>(strip[2], NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, D0>(strip[3], NUM_LEDS);
	randomSeed(analogRead(A0));
	defaultProg = false;

	for (int i = 0; i < NUM_LEDS; i++) {
		strip[0][i] = CRGB::Black;
	}
	FastLED.show();

    lastMinute = Time.minute();
    Particle.publish("Startup", String("System Version: " + System.version() + ", Program Version: " + APP_VERSION));
	Particle.function("program", setProgram);

    activeProgram = NO_PROGRAM;
    runAnyway = false;
    running = false;
    Serial.begin(115200);
}

void loop()
{
    Time.zone(currentTimeZone());
    sun.setPosition(LATITUDE, LONGITUDE, currentTimeZone());
    sun.setCurrentDate(Time.year(), Time.month(), Time.day());

    programOnDeck();

	switch (activeProgram) {
	case CHRISTMAS:
		runChristmas();
		break;
	case VALENTINES:
		runValentines();
		break;
	case INDEPENDENCE:
		runIndependence();
		break;
	case HALLOWEEN:
		runHalloween();
		break;
	case THANKSGIVING:
		runThanksgiving();
		break;
	case METEOR_SHOWER:
		runMeteorShower();
		break;
	case NORAH_BDAY:
		runNorah();
		break;
	case MADDIE_BDAY:
		runMaddie();
		break;
	case SNOW:
		runSnow();
		break;
	}
    printHeartbeat();
}

