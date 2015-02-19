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

