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

#include <TinyGPS++.h>
#include <FastLED.h>
#include <Time.h>
#include "pixelvector.h"
#include "LightsTypes.h"

#include "Christmas.h"
#include "Halloween.h"
#include "Valentines.h"
#include "IndependenceDay.h"
#include "Thanksgiving.h"
#include "SunPosition.h"

#define PIN1    5
#define PIN2    7
#define PIN3    9
#define PIN4    11

CRGB strip[NUM_STRIPS][NUM_LEDS];
bool runAnyway;
TinyGPSPlus gps;
HardwareSerial Uart = HardwareSerial();
SunPosition sun;

const int tzOffset =- 5;

void setup()
{
  runAnyway = false;
  pinMode(SWITCH_PIN, INPUT);
  attachInterrupt(SWITCH_PIN, isrService, FALLING);
  Uart.begin(9600);
  
  delay(3000);
  FastLED.addLeds<NEOPIXEL, PIN1>(strip[0], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN2>(strip[1], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN3>(strip[2], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN4>(strip[3], NUM_LEDS);
  randomSeed(analogRead(0));
}

bool validRunTime()
{
  double sunrise = sun.calcSunrise();
  double sunset = sun.calcSunset();
  double minsPastMidnight = gps.time.hour() * 60 + gps.time.minute();
  
  if ((minsPastMidnight >= (sunrise - 60)) || (minsPastMidnight <= (sunrise + 30))) {
    return true;
  }
  if ((minsPastMidnight >= (sunset - 30)) || gps.time.hour() != 0) {
    return true;
  }
  
  return false;
}

void runEncoder()
{
  while (Uart.available() > 0) {
    gps.encode(Uart.read());
  }
}

void isrService()
{
  __disable_irq();
  runAnyway = true;
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
  Christmas wink(TOTAL_PIXELS, NUM_ACTIVE);
  int buff = 0;
  
  Serial.println("Running Christmas");
  sun.disableDST();
  
  wink.startup();
  wink.setFirstActive(30);
  wink.seeTheRainbow();
  
  while (validRunTime()) {
    if (buff++ == 20) {
      buff = 0;
      wink.addOne();
    }
    wink.action();
    elapsedMillis delta;
    runEncoder();
    if (delta <= 25)
      delay(15 - delta);
  }
  pixelShutdown();
}

void runValentines()
{
  Valentines vday(TOTAL_PIXELS);
  vday.startup();
  
  sun.disableDST();
  
  while (validRunTime()) {
    vday.action();
    elapsedMillis delta;
    runEncoder();
    if (delta >= 500)
      delay(delta);
    else
      delay(500 - delta);
  }
  pixelShutdown();
}

void runIndependence()
{
  Independence iday(TOTAL_PIXELS);
  iday.startup();
  
  sun.enableDST();
  
  while (validRunTime()) {
    iday.action();
    elapsedMillis delta;
    runEncoder();
    if (delta >= 500)
      delay(delta);
    else
      delay(500 - delta);
  }
  pixelShutdown();
}

void runHalloween()
{
  Halloween hday(TOTAL_PIXELS);
  hday.startup();
  
  sun.enableDST();
  
  while (validRunTime()) {
    hday.action();
    elapsedMillis delta;
    runEncoder();
    if (delta >= 500)
      delay(delta);
    else
      delay(500 - delta);
  }
  pixelShutdown();
}

void runThanksgiving()
{
  Thanksgiving tday(TOTAL_PIXELS);
  tday.startup();
  
  sun.enableDST();
  
  while (validRunTime()) {
    tday.action();
    elapsedMillis delta;
    runEncoder();
    if (delta >= 500)
      delay(delta);
    else
      delay(500 - delta);
  }
  pixelShutdown();
}

int programOnDeck(int m, int d)
{
  if ((m == 12) && (d > 11)) {
    return CHRISTMAS;
  }
  if ((m == 2) && (d == 14)) {
    return VALENTINES;
  }
  if ((m == 7) && (d == 4)) {
    return INDEPENDENCE;
  }
  if ((m == 10) && (d > 24)) {
    return HALLOWEEN;
  }
  if (m == 11 && d > 20) {
    return THANKSGIVING;
  }
  if ((m == 5) && (d == 25)) {
    return MEMORIAL;
  }
  return NOHOLIDAY;
}

void setGPSData()
{
  setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
  adjustTime(tzOffset * SECS_PER_HOUR);
  sun.setPosition(gps.location.lat(), gps.location.lng(), tzOffset);
}

void loop()
{
  unsigned long fixAge;

  runEncoder();

  fixAge = gps.location.age();
  if (fixAge < 1500) {
    Serial.println("Valid fix detected");
    setGPSData();
  
    switch (programOnDeck(gps.date.month(), gps.date.day())) {
      case CHRISTMAS:
        runChristmas();
        break;
      case VALENTINES:
        runValentines();
        break;
      case MEMORIAL:
      case INDEPENDENCE:
        runIndependence();
        break;
      case HALLOWEEN:
        runHalloween();
        break;
      case THANKSGIVING:
        runThanksgiving();
        break;
      default:
        delay(500);
    }
  }
  else {
    Serial.println("No valid fix detected");
    delay(500);
  }
}
