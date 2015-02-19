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

Independence::Independence(int p)
{
  totalPixels = p;
  which = 0;
}

Independence::~Independence()
{
}

void Independence::startup()
{
  int window = totalPixels / 4;
  
  for (int i = 0; i < window; i++) {
    int c = i % 3;
    switch (c) {
      case 0:
        pixels.push_back(CRGB::Red);
        break;
      case 1:
        pixels.push_back(CRGB::White);
        break;
      case 2:
        pixels.push_back(CRGB::Blue);
        break;
    }
      
    for (int i = window; i < window * 2; i++) {
      pixels.push_back(CRGB::Blue);
    }
    for (int i = window * 2; i < window * 3; i++) {
      pixels.push_back(CRGB::White);
    }
    for (int i = window * 3; i < window * 4; i++) {
      pixels.push_back(CRGB::Red);
    }
  }
  seeTheRainbow();
}

void Independence::action()
{
  int window = totalPixels / 4;

  for (int i = 0; i < window; i++) {
    int c = i % 3;
    switch (c) {
      case 0:
        switch (which) {
          case 0:
            pixels[i] = CRGB::Red;
            break;
          case 1:
            pixels[i] = CRGB::White;
            break;
          case 2:
            pixels[i] = CRGB::Blue;
            break;
        }
        break;
      case 1:
        switch (which) {
          case 0:
            pixels[i] = CRGB::Blue;
            break;
          case 1:
            pixels[i] = CRGB::Red;
            break;
          case 2:
            pixels[i] = CRGB::White;
            break;
        }
        break;
      case 2:
        switch (which) {
          case 0:
            pixels[i] = CRGB::White;
            break;
          case 1:
            pixels[i] = CRGB::Blue;
            break;
          case 2:
            pixels[i] = CRGB::Red;
            break;
        }
        break;
    }
  }
  if (which == 2)
    which = 0;
  else
    which++;
    
  seeTheRainbow();
}

void Independence::seeTheRainbow()
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

