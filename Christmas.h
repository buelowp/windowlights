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
#ifndef __CHRISTMAS_H__
#define __CHRISTMAS_H__

#include "FastLED-sparkcore/firmware/FastLED.h"
#include "pixelvector.h"
#include "WindowLights.h"

#define GOING_UP        0
#define GOING_DOWN      1
#define RETURN_TO_NORM  2

class Christmas {
public:
  Christmas(int, int);
  ~Christmas();
  
  void startup();
  void action();
  void setFirstActive(int);
  void addOne();
  void seeTheRainbow();
  NSFastLED::CHSV getNextPixel();
  
private:
  bool scale_pixel_up(int i);
  bool scale_pixel_down(int i);
  bool scale_pixel_to_normal(int i);
  void set_new_pixel_color(int i);
  
  std::vector<NSFastLED::CHSV> pixels;
  LedPixelMap pixelMap;
  int totalPixels;
  int numActive;
  int index;
  int swap;
};

#endif

