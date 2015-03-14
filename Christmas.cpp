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
#include "Christmas.h"

#define SCALE_VAL       2
#define SCALE_SAT       3
#define SCALE_VAL_NORM  2

static HSVHue ChristmasColorWheel[] = {
  HUE_RED,
  HUE_YELLOW,
  HUE_BLUE,
  HUE_GREEN,
  HUE_PURPLE,
  HUE_ORANGE,
};

Christmas::Christmas(int p, int a)
{
  pixelMap.setMaxElements(a);
  totalPixels = p;
  numActive = a;
  index = 0;
  swap = random(2, 9);
}

Christmas::~Christmas()
{
}

bool Christmas::scale_pixel_up(int i)
{
  CHSV pixel = pixels[i];
  if (pixel.v == 255 && pixel.s == 0)
    return true;
    
  if ((pixel.v + SCALE_VAL) >= 255)
    pixel.v = 255;
  else
    pixel.v += SCALE_VAL;
    
  if ((pixel.s - SCALE_SAT) <= 0)
    pixel.s = 0;
  else
    pixel.s -= SCALE_SAT;
    
  pixels[i] = pixel;
  return false;
}

bool Christmas::scale_pixel_down(int i)
{
  CHSV pixel = pixels[i];
  if (pixel.v == 0 && pixel.s == 255)
    return true;
    
  if ((pixel.v - 4) <= 0)
    pixel.v = 0;
  else
    pixel.v -= 4;
    
  if ((pixel.s + 4) >= 255)
    pixel.s = 255;
  else
    pixel.s += 4;
    
  pixels[i] = pixel;
  return false;
}

bool Christmas::scale_pixel_to_normal(int i)
{
  CHSV pixel = pixels[i];
  if ((pixel.v + SCALE_VAL_NORM) >= NORMAL_BRIGHT) {
    pixel.v = NORMAL_BRIGHT;
    return true;
  }
  
  pixel.v += SCALE_VAL_NORM;
  
  pixels[i] = pixel;
  return false;
}

void Christmas::set_new_pixel_color(int i)
{
  pixels[i].v = 0;
  pixels[i].h = ChristmasColorWheel[random(0, NUM_COLORS)];
  pixels[i].s = 255;
}

void Christmas::startup()
{
  for (int i = 0; i < totalPixels; i++) {
    CHSV c;
    c.h = ChristmasColorWheel[random(0, NUM_COLORS)];
    c.s = 255;
    c.v = NORMAL_BRIGHT;
    pixels.push_back(c);
  }
}

/**
 * Initially set 20 pixels active and getting brighter. This number of
 * active pixels might be dynamic, but should grow to be about 20 - 25% of
 * all the pixels
 */
void Christmas::setFirstActive(int c)
{
  /*
  int count = 0;
  bool found = false;
  
  while (count < c) {
    int pixel = random(0, TOTAL_PIXELS);
    for (int j = 0; j < pixelMap.size(); j++) {
      if (pixelMap[j] == pixel)
        found = true;
        break;
    }
    if (!found) {
      pixelMap.addNewPixel(pixel, GOING_UP);
      found = false;
      count++;
    }
  }
  */
  pixelMap.addNewPixel(1, GOING_UP);
  pixelMap.addNewPixel(2, GOING_UP);
  pixelMap.addNewPixel(3, GOING_UP);
  pixelMap.addNewPixel(4, GOING_UP);
  pixelMap.addNewPixel(5, GOING_UP);
}

void Christmas::action()
{
  for (int i = 0; i <= pixelMap.size(); i++) {
    switch (pixelMap.pixelDir(i)) {
    case GOING_UP:
      if (scale_pixel_up(pixelMap[i]))
        pixelMap.setPixelDir(i, GOING_DOWN);

      break;
    case GOING_DOWN:
      if (scale_pixel_down(pixelMap[i])) {
        set_new_pixel_color(pixelMap[i]);
        pixelMap.setPixelDir(i, RETURN_TO_NORM);
      }
      break;
    case RETURN_TO_NORM:
      if (scale_pixel_to_normal(pixelMap[i]))
        pixelMap.removePixel(i);
        
      break;
    }    
  }
  seeTheRainbow();
}

void Christmas::addOne()
{
  int pixel = random(0, TOTAL_PIXELS);
  
  for (int i = 0; i < pixelMap.size(); i++) {
    if (pixelMap[i] == pixel)
      return;
  }
  if (pixelMap.size() < NUM_ACTIVE) {
    pixelMap.addNewPixel(pixel, GOING_UP);
  }
}

void Christmas::seeTheRainbow()
{
  for (int i = 0; i < NUM_STRIPS; i++) {
    CHSV s[NUM_LEDS];
    for (int j = 0; j < NUM_LEDS; j++) {
      s[j] = pixels[(i + 1) * j];
    }
    hsv2rgb_rainbow(s, strip[i], NUM_LEDS);
  }
  FastLED.show();
}


