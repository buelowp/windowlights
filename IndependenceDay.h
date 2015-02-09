#ifndef __INDEPENDENCE_H__
#define __INDEPENDENCE_H__

#include "FastLED.h"

class Independence {
public:
  Independence(int);
  ~Independence();
  
  void seeTheRainbow();
  void startup();
  void action();
  
private:
  Pixels<CRGB> *pixels;
  int totalPixels;
  int which;
};

#endif

