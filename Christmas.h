#ifndef __CHRISTMAS_H__
#define __CHRISTMAS_H__

#include "FastLED.h"

HSVHue ColorWheel[] = {
  HUE_RED,
  HUE_YELLOW,
  HUE_BLUE,
  HUE_GREEN,
  HUE_PURPLE,
  HUE_ORANGE,
};

#define GOING_UP        0
#define GOING_DOWN      1
#define RETURN_TO_NORM  2

class Christmas {
public:
  Christmas(int, int);
  ~Christmas();
  
  void startup();
  void action();
  void setFirstActive();
  void addOne();
  void seeTheRainbow();
  
  int 
  
private:
  Pixels<CHSV> *pixels;
  LedPixelMap *pixelMap;
  int totalPixels;
  int numActive;
  int index;
};

#endif
