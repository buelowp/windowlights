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

#define APP_VERSION			"2.7"

CRGB strip[NUM_STRIPS][LEDS_PER_STRIP];
SunSet sun;
bool runAnyway;
int lastMinute;
int localActiveProgram;
bool running;

const TProgmemRGBPalette16 Christmas_p =
{
		CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::Orange,
		CRGB::Purple, CRGB::Yellow, CRGB::White, CRGB::Blue,
		CRGB::Blue, CRGB::Blue, CRGB::Green, CRGB::Orange,
		CRGB::Yellow, CRGB::Red, CRGB::Purple, CRGB::Red
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

#define C9_Red 0xB80400
#define C9_Orange 0x902C02
#define C9_Green 0x046002
#define C9_Blue 0x070758
#define C9_White 0x606830
const TProgmemRGBPalette16 ClassicC9_p =
{
		C9_Red, C9_Red, C9_Red,
		C9_Orange, C9_Orange, C9_Orange,
		C9_Green, C9_Green, C9_Green, C9_Green,
		C9_Blue, C9_Blue, C9_Blue,
		C9_Red, C9_Blue, C9_White
};

const TProgmemRGBPalette16 NYE_p =
{
		CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold,
		CRGB::Silver, CRGB::Silver, CRGB::Silver, CRGB::Silver,
		CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Gold,
		CRGB::Silver, CRGB::Silver, CRGB::Silver, CRGB::Silver
};

const TProgmemRGBPalette16 Valentines_p =
{
		CRGB::Red, CRGB::DeepPink, CRGB::White, CRGB::Silver,
		CRGB::Red, CRGB::DeepPink, CRGB::White, CRGB::DeepPink,
		CRGB::Red, CRGB::DeepPink, CRGB::White, CRGB::Silver,
		CRGB::Red, CRGB::Gold, CRGB::White, CRGB::DeepPink,
};

const TProgmemRGBPalette16 Norah_p =
{
		CRGB::DeepPink, CRGB::Gold, CRGB::DeepPink, CRGB::Gold,
		CRGB::DeepPink, CRGB::Gold, CRGB::DeepPink, CRGB::Gold,
		CRGB::DeepPink, CRGB::Gold, CRGB::DeepPink, CRGB::Gold,
		CRGB::DeepPink, CRGB::Gold, CRGB::DeepPink, CRGB::Gold,
};

const TProgmemRGBPalette16 Maddie_p =
{
		CRGB::Aqua, CRGB::Lavender, CRGB::Aqua, CRGB::Lavender,
		CRGB::Aqua, CRGB::Lavender, CRGB::Aqua, CRGB::Lavender,
		CRGB::Aqua, CRGB::Lavender, CRGB::Aqua, CRGB::Lavender,
		CRGB::Aqua, CRGB::Lavender, CRGB::Aqua, CRGB::Lavender,
};

Christmas wink(NUM_LEDS, NUM_ACTIVE);
Twinkles twink(ClassicC9_p);
Twinkles snow(Snow_p);
Twinkles nye(NYE_p);
Twinkles vday(Valentines_p);
Independence iday(NUM_LEDS);
Halloween hday(NUM_LEDS);
Thanksgiving tday(NUM_LEDS);
Twinkles nbday(Norah_p);
Twinkles mbday(Maddie_p);
MeteorShower meteorStrip1;
MeteorShower meteorStrip2;
MeteorShower meteorStrip3;
MeteorShower meteorStrip4;

const uint8_t _usDSTStart[22] = { 8,13,12,11,10, 8,14,13,12,10, 9, 8,14,12,11,10, 9,14,13,12,11, 9};
const uint8_t _usDSTEnd[22]   = { 1, 6, 5, 4, 3, 1, 7, 6, 5, 3, 2, 1, 7, 5, 4, 3, 2, 7, 6, 5, 4, 2};

STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));

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

	// It's Christmas, just run them all day
	if (Time.month() == 12 && Time.day() == 25)
		return true;

	if ((minsPastMidnight >= 240) && (minsPastMidnight < (sunrise + 15))) {
		return true;
	}
	if (minsPastMidnight >= (sunset - 45) && (minsPastMidnight < 1380)) {
		return true;
	}

	return false;
}

void pixelShutdown()
{
	FastLED.clear();
	FastLED.show();
}

void runSnow()
{
	snow.setDensity(8);
	snow.setSpeed(6);

	snow.action();
	snow.seeTheRainbow();
}

void runChristmas()
{
	if ((Time.day() % 2)) {
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
			twink.setDensity(8);
			twink.setSpeed(4);
		}
		if (validFullDayRunTime() && running) {
			twink.action();
			twink.seeTheRainbow();
		}
		if (!validFullDayRunTime() && running) {
			pixelShutdown();
			running = false;
		}
	}
}

void runValentines()
{
	if (validNightRunTime() && !running) {
		running = true;
		vday.setDensity(8);
		vday.setSpeed(6);
	}
	if (validNightRunTime() && running) {
		vday.action();
		vday.seeTheRainbow();
	}
	if (!validNightRunTime() && running) {
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
	if (validFullDayRunTime() && !running) {
		running = true;
		nbday.setDensity(8);
		nbday.setSpeed(4);
	}
	if (validFullDayRunTime() && running) {
		nbday.action();
		nbday.seeTheRainbow();
	}
	if (!validFullDayRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runMaddie()
{
	if (validFullDayRunTime() && !running) {
		running = true;
		mbday.setDensity(8);
		mbday.setSpeed(5);
	}
	if (validFullDayRunTime() && running) {
		mbday.action();
		mbday.seeTheRainbow();
	}
	if (!validFullDayRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runMeteorShower()
{
	if (validFullDayRunTime() && !running) {
		running = true;
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

void runNYE()
{
	nye.action();
	nye.seeTheRainbow();
}

void runDefault()
{
	runIndependence();
}

int programOnDeck()
{
	switch (Time.month()) {
	case 1:
		if (Time.day() == 1)
			localActiveProgram = NEW_YEARS;
		break;
	case 2:
		if (Time.day() == 14)
			localActiveProgram = VALENTINES;
		break;
	case 4:
		if (Time.day() == 14)
			localActiveProgram = NORAH_BDAY;
		break;
	case 5:
		if (Time.day() > 24 && Time.weekday() == 2)
			localActiveProgram = INDEPENDENCE;
		break;
	case 7:
		if (Time.day() == 4)
			localActiveProgram = INDEPENDENCE;
		break;
	case 9:
		if (Time.day() == 17)
			localActiveProgram = MADDIE_BDAY;
		if (Time.day() < 8 && Time.weekday() == 2)
			localActiveProgram = INDEPENDENCE;
		break;
	case 10:
		if (Time.day() > 24)
			localActiveProgram = HALLOWEEN;
		break;
	case 11:
		if (Time.day() > 20)
			localActiveProgram = THANKSGIVING;
		break;
	case 12:
		if (Time.day() < 31)
			localActiveProgram = CHRISTMAS;
		if (Time.day() == 31)
			localActiveProgram = NEW_YEARS;
		break;
	}
	return localActiveProgram;
}

void printHeartbeat()
{
    if (lastMinute == 59 && Time.minute() >= 0) {
        Particle.publish("Heartbeat", String("System Version: " + System.version() + ", Program Version: " + APP_VERSION + ", Curr Program: " + String(localActiveProgram)));
        lastMinute = Time.minute();
    }

    if (Time.minute() >= lastMinute + 1) {
        Particle.publish("Heartbeat", String("System Version: " + System.version() + ", Program Version: " + APP_VERSION + ", Curr Program: " + String(localActiveProgram)));
        lastMinute = Time.minute();
    }
}

int setProgram(String prog)
{
	runAnyway = true;
	if (prog.equalsIgnoreCase("christmas")) {
		localActiveProgram = CHRISTMAS;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("newyears")) {
		localActiveProgram = NEWYEARS;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("halloween")) {
		localActiveProgram = HALLOWEEN;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("thanksgiving")) {
		localActiveProgram = THANKSGIVING;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("independence")) {
		localActiveProgram = INDEPENDENCE;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("maddie")) {
		localActiveProgram = MADDIE_BDAY;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("norah")) {
		localActiveProgram = NORAH_BDAY;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("meteors")) {
		localActiveProgram = METEOR_SHOWER;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("valentines")) {
		localActiveProgram = VALENTINES;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("snow")) {
		localActiveProgram = SNOW;
		return localActiveProgram;
	}
	if (prog.equalsIgnoreCase("nye")) {
		localActiveProgram = NEW_YEARS;
		return localActiveProgram;
	}
	pixelShutdown();
	runAnyway = false;
	running = false;
	localActiveProgram = NO_PROGRAM;
	return NO_PROGRAM;
}

void setup()
{
	pinMode(D0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);

	waitUntil(WiFi.ready);

	delay(3000);
	FastLED.addLeds<NEOPIXEL, 3>(strip[0], NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, 2>(strip[1], NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, 1>(strip[2], NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, D0>(strip[3], NUM_LEDS);
	randomSeed(analogRead(A0));

	Serial.begin(115200);
    lastMinute = Time.minute();
    Particle.publish("Startup", String("System Version: " + System.version() + ", Program Version: " + APP_VERSION));
	Particle.function("program", setProgram);

    localActiveProgram = NO_PROGRAM;
    runAnyway = false;
    running = false;
    FastLED.clear();
    FastLED.show();
}

void loop()
{
    Time.zone(currentTimeZone());
    sun.setPosition(LATITUDE, LONGITUDE, currentTimeZone());
    sun.setCurrentDate(Time.year(), Time.month(), Time.day());

	switch (programOnDeck()) {
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
	case NEW_YEARS:
		runNYE();
		break;
	}
    printHeartbeat();
}

