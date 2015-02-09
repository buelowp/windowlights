#include "IndependenceDay.h"

Independence::Independence(int p)
{
  pixels = new Pixels<CRGB>(p);
  totalPixels = p;
  which = 0;
}

Independence::~Independence()
{
  delete pixels;
}

void Independence::startup()
{
  int window = totalPixels / 4;
  
  for (int i = 0; i < window; i++) {
    int c = i % 3;
    switch (c) {
      case 0:
        pixels->push_back(CRGB::Red);
        break;
      case 1:
        pixels->push_back(CRGB::White);
        break;
      case 2:
        pixels->push_back(CRGB::Blue);
        break;
    }
    for (int i = window; i < window * 2; i++) {
      pixels->push_back(CRGB::Blue);
    }
    for (int i = window * 2; i < window * 3; i++) {
      pixels->push_back(CRGB::White);
    }
    for (int i = window * 3; i < window * 4; i++) {
      pixels->push_back(CRGB::Red);
    }
    seeTheRainbow();
}

void Independence::action()
{
  for (int i = 0; i < window; i++) {
    int c = i % 3;
    switch (c) {
      case 0:
        switch (which) {
          case 0:
            pixels[i] = CRGB::Red;
            break;
          case 1:
            pixels[i] = CRGB::White;
            break;
          case 2:
            pixels[i] = CRGB::Blue;
            break;
        }
        break;
      case 1:
        switch (which) {
          case 0:
            pixels[i] = CRGB::Blue;
            break;
          case 1:
            pixels[i] = CRGB::Red;
            break;
          case 2:
            pixels[i] = CRGB::White;
            break;
        }
        break;
      case 2:
        switch (which) {
          case 0:
            pixels[i] = CRGB::White;
            break;
          case 1:
            pixels[i] = CRGB::Blue;
            break;
          case 2:
            pixels[i] = CRGB::Red;
            break;
        }
        break;
    }
  }
  if (which == 2)
    which = 0;
  else
    which++;
    
  seeTheRainbow();
}

void Independence::seeTheRainbow()
{
  int index = -1;
  for (int i = 0; i < totalPixels; i++) {
    if ((i % NUM_LEDS) == 0)
      index++;
      
    strip[index][i % NUM_LEDS] = pixels[i];
  }
  FastLED.setBrightness(100);
  FastLED.show();
}

