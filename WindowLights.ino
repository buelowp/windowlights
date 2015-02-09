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

#include <Adafruit_GPS.h>
#include <FastLED.h>
#include <pixelvector.h>
#include <Time.h>

#include "Christmas.h"
#include "Halloween.h"
#include "Valentines.h"
#include "IndependenceDay.h"
#include "SunPosition.h"

#define SWITCH_PIN     2
#define LED_STRIP_1    3
#define LED_STRIP_2    4
#define LED_STRIP_3    5
#define LED_STRIP_4    6
#define GPS_RX_PIN     7

#define NUM_LEDS        115
#define NUM_STRIPS      4
#define NUM_ACTIVE      60
#define NORMAL_BRIGHT   100
#define NUM_COLORS      6
#define TOTAL_PIXELS    (NUM_LEDS * NUM_STRIPS)
#define NON_PIXEL       -1

#define CHRISTMAS      100
#define HALLOWEEN      101
#define THANKSGIVING   102
#define VALENTINES     103
#define MEMORIAL       104
#define INDEPENDENCE   105
#define NOHOLIDAY      999

bool runAnyway;
CRGB strip[NUM_STRIPS][NUM_LEDS];
double latitude;
double longitude;

void setup()
{
  runAnyway = false;
  pinMode(SWITCH_PIN, INPUT);
  attachInterrupt(SWITCH_PIN, isrService, FALLING);
  
  delay(3000);
  FastLED.addLeds<NEOPIXEL, PIN1>(strip[0], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN2>(strip[1], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN3>(strip[2], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN4>(strip[3], NUM_LEDS);
  randomSeed(analogRead(0));
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
  Winker wink(TOTAL_PIXELS, NUM_ACTIVE);
  int buff = 0;
  
  wink.startup();
  wink.setFistActive();
  wink.seeTheRainbow();
  
  while (whatTimeIsIt() == CHRISTMAS) {
    if (buff++ == 20) {
      buff = 0;
      wink.addOne();
    }
    wink.action();
    delay(25);
  }
  pixelShutdown();
}

void runValentines()
{
  Valentines vday(TOTAL_PIXELS);
  vday.startup();
  
  while (whatTimeIsIt() == VALENTINES) {
    vday.action();
    delay(500);
  }
  pixelShutdown();
}

void runIndependence()
{
  Independence iday(TOTAL_PIXELS);
  iday.startup();
  
  while (whatTimeIsit() == INDEPENDENCE) {
    iday.action();
    delay(500);
  }
  pixelShutdown();
}

void runHalloween()
{
  Halloween hday(TOTAL_PIXELS);
  hday.startup;
  
  while (whatTimeIsIt() == HALLOWEEN) {
    hday.action();
    delay(500);
  }
  pixelShutdown();
}

void programOnDeck()
{
  int m = gpsGetCurrentMonth();
  int d = gpsGetCurrentDay();
  
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
  if (m == 11) {
    if (d < 2) {
      return HALLOWEEN;
    }
    if (d > 20) {
      return THANKSGIVING;
    }
  }
  if ((m == 5) && (d == 25) {
    return MEMORIAL;
  }
  return NOHOLIDAY;
}

void whatTimeIsIt()
{
  time_t timeNow;
  SunPosition sun(latitude, longitude);
  
  sun.setCurrentDate();
  timeNow = gpsGetCurrentTime();
  if (sun.isSunset(timeNow)) {
    return programOnDeck();
  }
}

void loop()
{
  switch (whatTimeIsIt()) {
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
    default:
      delay(60000);
  }
}
