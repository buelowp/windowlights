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

SYSTEM_MODE(AUTOMATIC);

using namespace NSFastLED;

CRGB strip[NUM_STRIPS][LEDS_PER_STRIP];
SunSet sun;
int bounce;
bool defaultProg;
bool runAnyway;
int lastMinute;
int activeProgram;
bool running;
bool firstrun;

Christmas wink(NUM_LEDS, NUM_ACTIVE);

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

bool validRunTime()
{
	double sunrise = sun.calcSunrise();
	double sunset = sun.calcSunset();
	double minsPastMidnight = Time.hour() * 60 + Time.minute();

	return true;
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

void isrService()
{
	__disable_irq();
	if ((millis() - bounce) < 1000) {
		defaultProg = true;
		bounce = 0;
	}
	else {
		bounce = millis();
	}
	__enable_irq();
}

void pixelShutdown()
{
	for (int i = 0; i < NUM_STRIPS; i++) {
		for (int j = 0; j < NUM_LEDS; j++) {
			strip[i][j] = CRGB::Black;
		}
	}
	FastLED.show();
}

void runChristmas()
{
	if (validRunTime() && !running) {
		running = true;
		wink.startup();
		wink.setFirstActive(20);
		wink.seeTheRainbow();
	}
	if (validRunTime() && running) {
		wink.action();
		if (random(0, 3) == 2)
			wink.addOne();
		delay(25);
	}
	if (!validRunTime() && running) {
		pixelShutdown();
		running = false;
	}
}

void runValentines()
{
	Valentines vday(NUM_LEDS);
	if (validRunTime()) {
		vday.startup();
  
		while (validRunTime()) {
			vday.action();
			delay(500);
		}
		pixelShutdown();
	}
}

void runIndependence()
{
	Independence iday(NUM_LEDS);
	if (validRunTime()) {
		iday.startup();
    
		while (validRunTime()) {
			iday.action();
			delay(500);
		}
		pixelShutdown();
	}
}

void runHalloween()
{
	Halloween hday(NUM_LEDS);

	if (validRunTime()) {
		hday.startup();
    
		while (validRunTime()) {
			hday.action();
			if (random(0, 50) == 23) {
				hday.lightning(random(0, NUM_STRIPS));
			}
			delay(1000);
		}
		pixelShutdown();
	}
}

void runThanksgiving()
{
	Thanksgiving tday(NUM_LEDS);
	if (validRunTime()) {
		tday.startup();
    
		while (validRunTime()) {
			tday.action();
			delay(1000);
		}
		pixelShutdown();
	}
}

void runNorah()
{
	Norah bday(NUM_LEDS);
	if (validRunTime()) {
		bday.startup();
    
		while (validRunTime()) {
			bday.action();
			delay(500);
		}
		pixelShutdown();
	}
}

void runMaddie()
{
	Maddie bday(NUM_LEDS);
	if (validRunTime()) {
		bday.startup();

		while (validRunTime()) {
			bday.action();
			delay(500);
		}
		pixelShutdown();
	}
}

void runMeteorShower()
{
	MeteorShower meteorStrip1;
	MeteorShower meteorStrip2;
	MeteorShower meteorStrip3;
	MeteorShower meteorStrip4;

	while (validRunTime()) {
		meteorStrip1.action();
		meteorStrip2.action();
		meteorStrip3.action();
		meteorStrip4.action();

		meteorStrip1.seeTheRainbow();
		meteorStrip2.seeTheRainbow();
		meteorStrip3.seeTheRainbow();
		meteorStrip4.seeTheRainbow();
	}
	activeProgram = NO_PROGRAM;
	pixelShutdown();
}

void runDefault()
{
	Independence iday(NUM_LEDS);
	if (validRunTime()) {
		iday.startup();
    
		while (Time.hour() != 0) {
			iday.action();
			delay(500);
		}
		defaultProg = false;
		pixelShutdown();
	}
}

void programOnDeck()
{
	if (activeProgram != NO_PROGRAM)
		return;

	/* Christmas lights start on the 10th of december, and go through the end of the month */
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
        Particle.publish("Active Program", String("Active Program: " + String(activeProgram)));
        Particle.publish("Current Date", String("Current Date:" + String(Time.month()) + "-" + String(Time.day())));
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
	runAnyway = false;
	return NO_PROGRAM;
}

void setup()
{
	pinMode(SWITCH_PIN, INPUT);
	attachInterrupt(SWITCH_PIN, isrService, CHANGE);
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
	bounce = 0;
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
    firstrun = false;
    Serial.begin(115200);
    Serial.println("Startup finished");
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
	}
    printHeartbeat();
}

