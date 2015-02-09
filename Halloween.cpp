#include "Halloween.h"

Halloween::Halloween(int p)
{
  pixels = new Pixels<CRGB>(p);
  totalPixels = p;
  which = 0;
}

Halloween::~Halloween()
{
  delete pixels;
}

void Halloween::startup()
{
  int window = 1;
  CRGB c;
  
  for (int i = 0; i < totalPixels; i++) {
    if (i % 115) {
      c = ColorWheel[window++];
    }
    pixels->push-back(c);
  }
  seeTheRainbow();          
}

void Halloween::action()
{
}

void Halloween::seeTheRainbow()
{
}

