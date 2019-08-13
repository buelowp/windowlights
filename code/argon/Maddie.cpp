/*
 * Maddie.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: pete
 */

#include "Maddie.h"

extern CRGB strip[NUM_STRIPS][LEDS_PER_STRIP];

static CRGB MaddieColorWheel[] = {
		CRGB::Aqua,
		CRGB::Indigo,
};

Maddie::Maddie(int p)
{
	totalPixels = p;
	which = 0;
}

Maddie::~Maddie()
{
}

void Maddie::startup()
{
	for (int i = 0; i < totalPixels; i++) {
		if ((i % 2) == 0) {
			pixels.push_back(MaddieColorWheel[0]);
	    }
	    else {
	    	pixels.push_back(MaddieColorWheel[1]);
	    }
	}

	seeTheRainbow();
}

void Maddie::action()
{
	for (int i = 0; i < totalPixels; i++) {
		if ((i % 2) == which) {
			pixels[i] = (MaddieColorWheel[which]);
		}
		else {
			pixels[i] = (MaddieColorWheel[!which]);
		}
	}
	which = !which;
	seeTheRainbow();

}

void Maddie::seeTheRainbow()
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
