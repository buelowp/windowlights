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

#include "pixelvector.h"

LedPixelMap::LedPixelMap()
{
  maxelements = 0;
}

LedPixelMap::LedPixelMap(int m)
{
  maxelements = m;
}

LedPixelMap::~LedPixelMap()
{
}

void LedPixelMap::setPixelDir(int i, int d)
{
  dir[i] = d;
}

void LedPixelMap::setPixel(int i, int p)
{
  pixel[i] = p;
}

void LedPixelMap::removePixel(int i)
{
  if (i < pixel.size()) {
    pixel.erase(pixel.begin() + i);
    dir.erase(dir.begin() + i);
  }
}

int LedPixelMap::addNewPixel(int p, int d)
{
  if (pixel.size() == maxelements)
    return -1;

  for (int i = 0; i < pixel.size(); i++) {
    if (pixel[i] == p)
      return -1;
  }
  pixel.push_back(p);
  dir.push_back(d);

  return pixel.size();
}


