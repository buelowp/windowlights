/*
 * Twinkles.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: pete
 */

#include "Twinkles.h"

Twinkles::Twinkles(CRGBPalette16 pal)
{
	gCurrentPalette = pal;
	gBackgroundColor = CRGB(16,14,4);
	density = 8;
	speed = 5;
}

Twinkles::~Twinkles()
{
}

void Twinkles::startup()
{
}

void Twinkles::setBackgroundColor(CRGB c)
{
	gBackgroundColor = c;
}

void Twinkles::setDensity(int d)
{
	if (d <= 8)
		density = d;
}

void Twinkles::setSpeed(int s)
{
	if (s <= 8)
		speed = s;
}

//  This function loops over each pixel, calculates the
//  adjusted 'clock' that this pixel should use, and calls
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color,
//  whichever is brighter.
void Twinkles::action()
{
	  // "PRNG16" is the pseudorandom number generator
	  // It MUST be reset to the same starting value each time
	  // this function is called, so that the sequence of 'random'
	  // numbers that it generates is (paradoxically) stable.
	uint16_t PRNG16 = 11337;
	uint32_t clock32 = millis();

	uint8_t backgroundBrightness = gBackgroundColor.getAverageLight();

	for (int j = 0; j < NUM_STRIPS; j++) {
		for ( uint16_t i = 0; i < LEDS_PER_STRIP; i++) {
			PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
			uint16_t myclockoffset16= PRNG16; // use that number as clock offset
			PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
			// use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
			uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
			uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
			uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

			// We now have the adjusted 'clock' for this pixel, now we call
			// the function that computes what color the pixel should be based
			// on the "brightness = f( time )" idea.
			CRGB c = computeOneTwinkle( myclock30, myunique8);

			// If the new pixel is brighter than the background color, use it.
			if( c.getAverageLight() > backgroundBrightness) {
				strip[j][i] = c;
			} else {
				strip[j][i] = gBackgroundColor;
			}
		}
	}
}

// This function is like 'triwave8',
// (which produces a
// symmetrical up-and-down triangle
// sawtooth waveform), except that this
// function produces a triangle wave with
// a faster attack and a slower decay:
//
uint8_t Twinkles::attackDecayWave8( uint8_t i)
{
	if (i < 86) {
		return (i * 3);
	} else {
		i -= 86;
		return (255 - (i + (i/2)));
	}
}

//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as
//  input to the brightness wave function.
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB Twinkles::computeOneTwinkle( uint32_t ms, uint8_t salt)
{
	uint16_t ticks = ms >> (8-speed);
	uint8_t fastcycle8 = ticks;
	uint8_t slowcycle8 = (ticks >> 8) ^ salt;

	uint8_t bright = 0;
	if( (slowcycle8 & 0x0E) < density) {
		bright = attackDecayWave8( fastcycle8);
	}

	uint8_t hue = (slowcycle8 * 16) + salt;
	return ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
}

void Twinkles::seeTheRainbow()
{
	FastLED.show();
}
