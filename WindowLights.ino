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

#include <TinyGPS.h>
#include <FastLED.h>
#include <Time.h>
#include "pixelvector.h"
#include "WindowLights.h"

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
TinyGPS gps;
HardwareSerial Uart = HardwareSerial();
SunPosition sun;
int bounce;
bool defaultProg;
int tzOffset;

void setup()
{
  pinMode(SWITCH_PIN, INPUT);
  attachInterrupt(SWITCH_PIN, isrService, FALLING);
  Uart.begin(9600);
  
  delay(3000);
  FastLED.addLeds<NEOPIXEL, PIN1>(strip[0], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN2>(strip[1], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN3>(strip[2], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN4>(strip[3], NUM_LEDS);
  randomSeed(analogRead(0));
  bounce = 0;
  defaultProg = false;
  tzOffset = 0;
}

int getDOW(int m, int d)
{
  int A = 0;
  int B = d;
  int C = 0;
  int D = 0;
  int W = 0;
  int X = 0;
  int Y = 0;
  int Z = 0;
  int R = 0;
  int newYear = 0;
  
  if (m < 3) {
    A = m + 10;
    newYear = year() - 1;
  }
  else {
    A = m - 2;
    newYear = year();
  }
  C = newYear % 100;
  D = newYear / 100;
  W = (13 * A - 1) / 5;
  X = C / 4;
  Y = D / 4;
  Z = W + X + Y + B + C - 2 * D;
  R = Z % 7;
  if (R < 0)
    R+=7;
    
  return R;
}

int whichDay(int m, int DOW, int NthWeek){
  int targetDate = 1;
  int firstDOW = getDOW(m, targetDate);
  
  while (firstDOW != DOW){
    firstDOW = (firstDOW+1)%7;
    targetDate++;
  }
  
  //Adjust for weeks
  targetDate += (NthWeek-1)*7;
  return targetDate;
}

void setDST()
{
  int springForward = whichDay(3, 0, 2);
  int fallBack = whichDay(11, 0, 1);
  
  switch (month()) {
    case 12:
    case 1:
    case 2:
      tzOffset = -6;
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      tzOffset = -5;
      break;
    case 3:
      if (day() >= springForward)
        tzOffset = -5;
      else
        tzOffset = -6;
      break;
    case 11:
      if (day() >= fallBack)
        tzOffset = -6;
      else
        tzOffset = -5;
      break;
  }
}

bool validRunTime()
{
  double sunrise = sun.calcSunrise();
  double sunset = sun.calcSunset();
  double minsPastMidnight = hour() * 60 + minute();
  
  if ((minsPastMidnight >= (sunrise - 60)) || (minsPastMidnight <= (sunrise + 30))) {
    return true;
  }
  if ((minsPastMidnight >= (sunset - 30)) || hour() != 0) {
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
  Christmas wink(TOTAL_PIXELS, NUM_ACTIVE);
  int buff = 0;
  
  Serial.println("Running Christmas");
  sun.setTZOffset(tzOffset);
  
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
      delay(25 - delta);
  }
  pixelShutdown();
}

void runValentines()
{
  Valentines vday(TOTAL_PIXELS);
  vday.startup();
  
  sun.setTZOffset(tzOffset);
  
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
  
  sun.setTZOffset(tzOffset);
  
  while (validRunTime()) {
    iday.action();
    elapsedMillis delta;
    runEncoder();
    if (delta >= 500)
      delay(delta);
    else {
      delay(500 - delta);
    }
  }
  pixelShutdown();
}

void runHalloween()
{
  Halloween hday(TOTAL_PIXELS);
  hday.startup();
  
  sun.setTZOffset(tzOffset);
  
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
  
  sun.setTZOffset(tzOffset);
  
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

void runDefault()
{
  Independence iday(TOTAL_PIXELS);
  iday.startup();
  
  while (hour() != 0) {
    iday.action();
    elapsedMillis delta;
    runEncoder();
    if (delta >= 500)
      delay(delta);
    else {
      delay(500 - delta);
    }
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

unsigned long runEncoder()
{
  unsigned long age;
  int Year;
  byte Month, Day, Hour, Minute, Second;
  long latitude, longitude;
  
  while (Uart.available() > 0) {
    gps.encode(Uart.read());
    gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, NULL, &age);
    if (age < 500) {
      gps.get_position(&latitude, &longitude);
      setTime(Hour, Minute, Second, Day, Month, Year);
      adjustTime(tzOffset * SECS_PER_HOUR);
      sun.setPosition(latitude, longitude, tzOffset);
    }
  }
  return age;
  
  void get_position(long *latitude, long *longitude, unsigned long *fix_age = 0);
}

void loop()
{
  unsigned long fixAge;

  fixAge = runEncoder();

  if (runDefault) {
    runDefault();
  }
  else {
    if (fixAge < 1500) {
      Serial.println("Valid fix detected");
    
      switch (programOnDeck(month(), day())) {
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
  }
}
