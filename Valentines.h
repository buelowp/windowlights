#ifndef __VALENTINES_H__
#define __VALENTINES_H__

#include "FastLED.h"

class Valentines {
public:
  Valentines(int);
  ~Valentines();
  
  void seeTheRainbow();
  void startup();
  void action();
  
private:
  Pixels<CRGB> *pixels;
  int totalPixels;
  int which;
};

#endif

