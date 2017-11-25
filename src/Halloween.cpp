/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015, Peter Buelow
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * The Lightning code was taken from Daniel Wilson
 * https://github.com/fibonacci162/LEDs/tree/master/Lightning2014
 *
 */
#include "Halloween.h"

#define FLASHES 15

static CRGB HalloweenColorWheel[] = {
	CRGB::Red,
	CRGB::OrangeRed,
	CRGB::Purple,
	CRGB::Green,
	CRGB::Blue,
};

Halloween::Halloween(int p)
{
  totalPixels = p;
  which = 0;
}

Halloween::~Halloween()
{
}

void Halloween::startup()
{
  CRGB c;

  for (int i = 0; i < NUM_STRIPS; i++) {
    c = HalloweenColorWheel[random(0, 5)];
    for (int j = 0; j < LEDS_PER_STRIP; j++) {
      strip[i][j] = c;
    }
  }
  
  seeTheRainbow();
}

/*
* Borrowed with all thanks from Daniel Wilson
* https://github.com/fibonacci162/LEDs/tree/master/Lightning2014
*/
void Halloween::lightning(int window)
{
	CHSV flasher[NUM_LEDS];
  uint8_t dimmer;
  for (int flashCounter = 0; flashCounter < random(6,FLASHES); flashCounter++)
  {
    if (flashCounter == 0)
      dimmer = 5; // the brightness of the leader is scaled down by a factor of 5
    else
      dimmer = random(1,3); // return strokes are brighter than the leader

    for (int i = 0; i < NUM_LEDS; i++) {
      flasher[i] = CHSV(255, 0, 255/dimmer);
    }
    hsv2rgb_rainbow(flasher, strip[window], NUM_LEDS);
    FastLED.show();
    delay(random(4,10)); // each flash only lasts 4-10 milliseconds
    for (int i = 0; i < NUM_LEDS; i++) {
      flasher[i] = CHSV(255, 0, 0);
    }
    hsv2rgb_rainbow(flasher, strip[window], NUM_LEDS);
    FastLED.show();
    
    if (flashCounter == 0)
      delay (150); // longer delay until next flash after the leader
  
    delay(50+random(100)); // shorter delay between strokes
  }
}

void Halloween::action()
{
  int window = random(0, 3);
  CRGB c = HalloweenColorWheel[random(0, 5)];
  
  for (int j = 0; j < LEDS_PER_STRIP; j++) {
    strip[window][j] = c;
  }
  seeTheRainbow();
}

void Halloween::seeTheRainbow()
{
	FastLED.setBrightness(100);
	FastLED.show();
}

