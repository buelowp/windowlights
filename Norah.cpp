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
#include "Norah.h"

static CRGB NorahColorWheel[] = {
  CRGB::DeepPink,
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
  for (int i = 0; i < totalPixels; i++) {
    if ((i % 2) == 0) {
      pixels.push_back(CRGB::DeepPink);
    }
    else {
      pixels.push_back(CRGB::Gold);
    }
  }
    
  seeTheRainbow();
}

void Norah::action()
{
  for (int i = 0; i < totalPixels; i++) {
    if ((i % 2) == which) {
      pixels[i] = (CRGB::DeepPink);
    }
    else {
      pixels[i] = (CRGB::Gold);
    }
  }
  which = !which;
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

