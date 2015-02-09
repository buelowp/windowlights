#ifndef __HALLOWEEN_H__
#define __HALLOWEEN_H__

#include "FastLED.h"

CRGB ColorWheel[] = {
  CRGB::Red,
  CRGB::OrangeRed,
  CRGB::Purple,
  CRGB::Green,
  CRGB::Blue,
};

class Halloween {
public:
  Halloween(int);
  ~Haloween();
  
  void seeTheRainbow();
  void startup();
  void action();
  
private:
  Pixels<CRGB> *pixels;
  int which;
  int totalPixels;
};

#endif
