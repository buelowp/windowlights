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
    strip[3][i] = CRGB::Black;
  }
  FastLED.show();

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
    runGPSDecoder();
    if (delta <= 25)
      delay(25 - delta);
  }
  pixelShutdown();
}

void runValentines()
{
  Valentines vday(TOTAL_PIXELS);
  vday.startup();
  Serial.println("In V-Day");
  sun.setTZOffset(tzOffset);
  
  while (validRunTime()) {
    vday.action();
    elapsedMillis delta;
    runGPSDecoder();
    if (delta >= 500) {
      delay(delta);
    }
    else {
      delay(500 - delta);
    }
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
    runGPSDecoder();
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
    runGPSDecoder();
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
    runGPSDecoder();
    if (delta >= 500)
      delay(delta);
    else
      delay(500 - delta);
  }
  pixelShutdown();
}

void runNorah()
{
  Norah bday(TOTAL_PIXELS);
  bday.startup();
  
  Serial.println("Running Norah");
  sun.setTZOffset(tzOffset);
  
  while (validRunTime()) {
    bday.action();
    elapsedMillis delta;
    runGPSDecoder();
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
    runGPSDecoder();
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
  if (year() < 2015) {
    Serial.print("Got year = ");
    Serial.println(year());
    return NOHOLIDAY;
  }
    
  Serial.print("Check for m=");
  Serial.print(m);
  Serial.print(" and day=");
  Serial.println(d);
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
  if ((m == 3) && (d == 10)) {
    return NORAH;
  }
  Serial.print("Failed date check for m=");
  Serial.print(m);
  Serial.print(" and day=");
  Serial.println(d);
  return NOHOLIDAY;
}

void runGPSDecoder()
{
  while (Serial1.available()) {
    if (gps.encode(Serial1.read())) { // process gps messages
      if (gps.time.isValid()) {
        setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
        setDST();
        adjustTime(tzOffset * SECS_PER_HOUR);
        sun.setCurrentDate(year(), month(), day());
      }
    }
  }
    if (timeStatus()!= timeNotSet) {
      if (now() != prevDisplay) { //update the display only if the time has changed
        if (gps.location.isValid()) {
          sun.setPosition(gps.location.lat(), -gps.location.lng(), tzOffset);
        }
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
  if (gps.location.isValid()) {
    sun.setPosition(gps.location.lat(), -gps.location.lng(), tzOffset);
  }
  else {
    return;
  }
  
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
        case NORAH:
          runNorah();
          break;
        default:
          delay(500);
      }

  }
}

