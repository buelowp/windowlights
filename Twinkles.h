/*
 * Twinkles.h
 *
 *  Created on: Dec 17, 2015
 *      Author: pete
 */

#ifndef USER_APPLICATIONS_WINDOWLIGHTS_TWINKLES_H_
#define USER_APPLICATIONS_WINDOWLIGHTS_TWINKLES_H_

#include "WindowLights.h"

class Twinkles {
public:
	Twinkles(CRGBPalette16);
	virtual ~Twinkles();

	void startup();
	void action();
	void setBackgroundColor(CRGB);
	void seeTheRainbow();

private:
	uint8_t attackDecayWave8(uint8_t);
	CRGB computeOneTwinkle( uint32_t ms, uint8_t salt);

	CRGBPalette16 gCurrentPalette;
	CRGB gBackgroundColor;
};

#endif /* USER_APPLICATIONS_WINDOWLIGHTS_TWINKLES_H_ */
