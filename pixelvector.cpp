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

