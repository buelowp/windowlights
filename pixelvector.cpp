#include "pixelvector.h"

template <class T> Pixels<T>::Pixels(int m)
{
  numelements = 0;
  maxelements = m;
  pixel = new T[m];
}

template <class T> Pixels<T>::~Pixels()
{
  delete[] pixel;
}

template <class T> int Pixels<T>::push_back(T element)
{
  if (numelements < maxelements) {
    pixel[numelements] = element;
    return ++numelements;
  }
  return -1;
}

template <class T> void Pixels<T>::set(T element, int index)
{
  if (index < maxelements) {
    pixel[index] = element;
  }
}

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

