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

#ifndef __LEDPIXELMAP_H__
#define __LEDPIXELMAP_H__

#include <vector>

using namespace std;

class LedPixelMap {
public:
  LedPixelMap(int m);
  LedPixelMap();
  ~LedPixelMap();

  int size() { return pixel.size(); }
  int pixelDir(int i) { return dir[i]; }
  inline int operator[](int i) { return pixel[i]; }
  void setPixelDir(int i, int d);
  void setPixel(int i, int p);
  void removePixel(int p);
  int addNewPixel(int p, int d);
  void setMaxElements(int m) { maxelements = m; }

private:
  int maxelements;
  vector<int> pixel;
  vector<int> dir;
};


#endif


