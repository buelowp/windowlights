#ifndef __LEDPIXELMAP_H__
#define __LEDPIXELMAP_H__

#include <vector>

using namespace std;

template <class T> class Pixels {
public:
  Pixels();
  Pixels(int m);
  ~Pixels();

  int size() { return numelements; }
  T at(int index) __attribute__((always_inline)) { return pixel[index]; }
  T operator[] (int i) __attribute__((always_inline)) { return pixel[i]; }
  int push_back(T element);
  void set(T element, int index);

private:
  int numelements;
  int maxelements;
  T *pixel;
};


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

