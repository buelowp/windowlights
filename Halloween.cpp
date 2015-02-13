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
#include "Halloween.h"

static CRGB HalloweenColorWheel[] = {
  CRGB::Red,
  CRGB::OrangeRed,
  CRGB::Purple,
  CRGB::Green,
  CRGB::Blue,
};

Halloween::Halloween(int p)
{
  pixels = Pixels<CRGB>(p);
  totalPixels = p;
  which = 0;
}

Halloween::~Halloween()
{
}

void Halloween::startup()
{
  int window = 1;
  CRGB c;
  
  for (int i = 0; i < totalPixels; i++) {
    if (i % 115) {
      c = HalloweenColorWheel[window++];
    }
    pixels.push_back(c);
  }
  seeTheRainbow();          
}

void Halloween::action()
{
}

void Halloween::seeTheRainbow()
{
}

