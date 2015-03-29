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
 */
#include "Thanksgiving.h"

static CRGB ThanksgivingColorWheel[] = {
  CRGB::Red,
  CRGB::OrangeRed,
  CRGB::Purple,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Yellow,
  CRGB::White,
};

Thanksgiving::Thanksgiving(int p)
{
  totalPixels = p;
  which = 0;
}

Thanksgiving::~Thanksgiving()
{
}

void Thanksgiving::startup()
{
  int window = 0;
  CRGB c;
  
  for (int i = 0; i < NUM_STRIPS; i++) {
    c = ThanksgivingColorWheel[random(0, 7)];
    for (int j = 0; j < LEDS_PER_STRIP; j++) {
      strip[i][j] = c;
    }
  }
  seeTheRainbow();          
}

void Thanksgiving::action()
{
  int window = random(0, 3);
  CRGB c = ThanksgivingColorWheel[random(0, 7)];
  
  for (int j = 0; j < LEDS_PER_STRIP; j++) {
    strip[window][j] = c;
  }
  seeTheRainbow(window);
}

void Thanksgiving::seeTheRainbow()
{
  int k = 0;
  
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < LEDS_PER_STRIP; j++) {
      strip[i][j] = pixels[k++];
    }
  }
  FastLED.setBrightness(100);
  FastLED.show();
}

void Thanksgiving::seeTheRainbow(int window)
{
  int k = 0;
  
  for (int j = 0; j < LEDS_PER_STRIP; j++) {
    strip[window][j] = pixels[k++];
  }
  FastLED.setBrightness(100);
  FastLED.show();
}


