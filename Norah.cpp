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
#include "IndependenceDay.h"

static CRGB NorahColorWheel[] = {
  CRGB::Pink,
  CRGB::Gold,
};

Norah::Norah(int p)
{
  totalPixels = p;
  which = 0;
}

Norah::~Norah()
{
}

void Norah::startup()
{
  int sep = totalPixels / 4;
  
  for (int i = 0; i < totalPixels; i++) {
    switch (i / sep) {
      case 0:
        pixels.push_back(CRGB::Pink);
        break;
      case 1:
        pixels.push_back(CRGB::Gold);
        break;
      case 2:
        pixels.push_back(CRGB::Pink);
        break;
      case 3:
        pixels.push_back(CRGB::Gold);
        break;
    }
    
  seeTheRainbow();
}

void Norah::action()
{
  int sep = totalPixels / 4;
  int first = 0;
  int sec = 1;
  
  for (int i = 0; i < totalPixels; i++) {
    switch (i / sep) {
      case 1:
      case 2:
        if ((i % 2) == 0) {
          pixels[i] = NorahColorWheel[first];
        }
        else {
          pixels[i] = NorahColorWheel[sec];
        }
        break;
    }
  }
  if (first)
    first = 0;
  else
    first = 1;
    
  if (sec)
    sec = 0;
  else
    sec = 1;
          
  seeTheRainbow();
}

void Norah::seeTheRainbow()
{
  int index = -1;
  for (int i = 0; i < totalPixels; i++) {
    if ((i % NUM_LEDS) == 0)
      index++;
      
    strip[index][i % NUM_LEDS] = pixels[i];
  }
  FastLED.setBrightness(100);
  FastLED.show();
}

