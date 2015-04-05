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
#include "WindowLights.h"

#include "Christmas.h"
#include "Halloween.h"
#include "Valentines.h"
#include "IndependenceDay.h"
#include "Thanksgiving.h"
#include "SunPosition.h"
#include "Norah.h"

#define PIN1    3
#define PIN2    4
#define PIN3    17
#define PIN4    18

CRGB strip[NUM_STRIPS][NUM_LEDS];
TinyGPSPlus gps;
time_t prevDisplay = 0; // when the digital clock was displayed
SunPosition sun;
int bounce;
bool defaultProg;
int tzOffset;
int delta;

void setup()
{
  pinMode(SWITCH_PIN, INPUT);
  attachInterrupt(SWITCH_PIN, isrService, FALLING);
  Serial1.begin(9600);
  Serial.begin(115200);
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  
  delay(3000);
//  FastLED.addLeds<NEOPIXEL, PIN1>(strip[0], NUM_LEDS);
//  FastLED.addLeds<NEOPIXEL, PIN2>(strip[1], NUM_LEDS);
//  FastLED.addLeds<NEOPIXEL, PIN3>(strip[2], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN4>(strip[0], NUM_LEDS);
  randomSeed(analogRead(0));
  bounce = 0;
  defaultProg = false;
  tzOffset = 0;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    strip[0][i] = CRGB::Black;
  }
  FastLED.show();

  delta = 0;
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
  
  if ((minsPastMidnight >= (sunrise - 60)) && (minsPastMidnight < (sunrise + 15))) {
    return true;
  }
  if (minsPastMidnight >= (sunset - 30) && (minsPastMidnight < 1440)) {
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
  
  if (validRunTime()) {
    wink.startup();
    wink.setFirstActive(5);
    wink.seeTheRainbow();
  
    while (validRunTime()) {
      wink.action();
      if (random(0, 3) == 2)
        wink.addOne();
        
      delay(25);
    }
    pixelShutdown();
  }
}

void runValentines()
{
  Valentines vday(TOTAL_PIXELS);
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
  Independence iday(TOTAL_PIXELS);
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
  Halloween hday(TOTAL_PIXELS);

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
  Thanksgiving tday(TOTAL_PIXELS);
  if (validRunTime()) {
    tday.startup();
    
    while (validRunTime()) {
      tday.action();
      delay(500);
    }
    pixelShutdown();
  }
}

void runNorah()
{
  Norah bday(TOTAL_PIXELS);
  if (validRunTime()) {
    bday.startup();
    
    while (validRunTime()) {
      bday.action();
      delay(500);
    }
    pixelShutdown();
  }
}

void runDefault()
{
  Independence iday(TOTAL_PIXELS);
  if (validRunTime()) {
    iday.startup();
    
    while (hour() != 0) {
      iday.action();
      delay(500);
    }
    defaultProg = false;
    pixelShutdown();
  }
}

int programOnDeck(int m, int d)
{
  if (year() < 2015) {
    return NOHOLIDAY;
  }
  
  /* Christmas lights start on the 10th of december, and go through the end of the month */
  if ((m == 12) && (d >= 10)) {
    return CHRISTMAS;
  }
  /* Valentines day lights only on the 14th */
  if ((m == 2) && (d == 14)) {
    return VALENTINES;
  }
  /* Independence day lights on the 4th, but also on Labor and Memorial days */
  if ((m == 7) && (d == 4)) {
    return INDEPENDENCE;
  }
  if (m == 5) {
    if ((d > 24) && (getDOW(m, d) == 1)) {
      return INDEPENDENCE;
    }
  }
  if (m == 9) {
    if ((d < 8) && (getDOW(m, d) == 1)) {
      return INDEPENDENCE;
    }
  }
  /* Halloween starts on the 24th and runs through the end of the month */
  if ((m == 10) && (d > 24)) {
    return HALLOWEEN;
  }
  /* Start turkey day lights on the 20th and run through the end of the month */
  if (m == 11 && d > 20) {
    return THANKSGIVING;
  }
  /* Random other days to show a pattern */
  if (m == 4) {
    if (d == 14)
      return HDAY1;
    if (d == 18)
      return HDAY1;
  }
  return NOHOLIDAY;
}

/**
 * Run the decoder for at least a few seconds. I found an issue
 * where it is possible to ignore GPS for a while, then the first
 * read will reset time to an odd or unexpected value. However, it
 * was valid, so the system would be confused as to what time
 * it really is.
 */
void runGPSDecoder()
{
  elapsedMillis howlong;
  
  while (howlong < 5000) {
    while (Serial1.available()) {
      if (gps.encode(Serial1.read())) { // process gps messages
        if (gps.time.isValid()) {
          setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
          setDST();
          adjustTime(tzOffset * SECS_PER_HOUR);
          sun.setCurrentDate(year(), month(), day());
        }
        if (gps.location.isValid()) {
          sun.setPosition(gps.location.lat(), -gps.location.lng(), tzOffset);
        }
      }
    }
  }
  
  if (timeStatus()!= timeNotSet) {
    if (now() != prevDisplay) { //update the display only if the time has changed
      prevDisplay = now();
      digitalClockDisplay();  
    }
  }
}

void digitalClockDisplay(){
  int dow = getDOW(month(), day());
  // digital clock display of the time
  
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print("/");
  Serial.print(year());
  Serial.print(" ");
  Serial.print(tzOffset); 

  switch (dow) {
    case 0:
      Serial.println(" Sunday");
      break;
    case 1:
      Serial.println(" Monday");
      break;
    case 2:
      Serial.println(" Tuesday");
      break;
    case 3:
      Serial.println(" Wednesday");
      break;
    case 4:
      Serial.println(" Thursday");
      break;
    case 5:
      Serial.println(" Friday");
      break;
    case 6:
      Serial.println(" Saturday");
      break;
    default:
      Serial.println(dow);
      break;
  }
}
void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void loop()
{
  runGPSDecoder();

  if (defaultProg) {
    runDefault();
  }
  else {
    switch (programOnDeck(month(), day())) {
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
      case HDAY1:
        runNorah();
        break;
    }
  }
}

