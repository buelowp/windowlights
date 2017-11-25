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
}

Independence::~Independence()
{
}

void Independence::startup()
{
  int which = 0;
  
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < LEDS_PER_STRIP; j++) {
      switch (i) {
        case 0:
          strip[i][j] = CRGB::Red;
          break;
        case 1:
          strip[i][j] = CRGB::White;
          break;
        case 2:
          strip[i][j] = CRGB::Blue;
          break;
        case 3:
          switch (which) {
            case 0:
              strip[i][j] = CRGB::Red;
              which++;
              break;
            case 1:
              strip[i][j] = CRGB::White;
              which++;
              break;
            case 2:
              strip[i][j] = CRGB::Blue;
              which = 0;
              break;
          }
        break;
      }
    }
  }
  seeTheRainbow();
}

void Independence::action()
{
  int which = 0;
  
  for (int i = 0; i < LEDS_PER_STRIP; i++) {
    int c = i % 3;
    switch(c) {
      case 0:
        switch (which) {
          case 0:
            strip[0][i] = CRGB::Red;
            break;
          case 1:
            strip[0][i] = CRGB::White;
            break;
          case 2:
            strip[0][i] = CRGB::Blue;
            break;
        }
        break;
      case 1:
        switch (which) {
          case 0:
            strip[0][i] = CRGB::Blue;
            break;
          case 1:
            strip[0][i] = CRGB::Red;
            break;
          case 2:
            strip[0][i] = CRGB::White;
            break;
        }
        break;
      case 2:
        switch (which) {
          case 0:
            strip[0][i] = CRGB::White;
            break;
          case 1:
            strip[0][i] = CRGB::Blue;
            break;
          case 2:
            strip[0][i] = CRGB::Red;
            break;
        }
        break;
    }
  }
  seeTheRainbow();
}

void Independence::seeTheRainbow()
{
	FastLED.setBrightness(100);
	FastLED.show();
}
