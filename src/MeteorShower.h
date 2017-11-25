/*
 * MeteorShower.h
 *
 *  Created on: Nov 9, 2015
 *      Author: pete
 */

#ifndef USER_APPLICATIONS_WINDOWLIGHTS_METEORSHOWER_H_
#define USER_APPLICATIONS_WINDOWLIGHTS_METEORSHOWER_H_

#include "WindowLights.h"

class MeteorShower {
public:
	MeteorShower();
	virtual ~MeteorShower();
	void setActiveStrip(CRGB[]);
	void setActiveFramerate(int f) { frameDelay = f; }
	void seeTheRainbow();
	void startup();
	void action();

private:
	byte adjacentCCW(byte);

	CRGB leds[LEDS_PER_STRIP];
	byte ledsX[LEDS_PER_STRIP][3];
	bool RAINBOWs = false;
	bool RANDOMpick = false;
	uint8_t hue;
	uint16_t timeframe;
	byte idex = 0;
	byte colorTIP = 0;
	byte meteorLENGTH;
	byte loopCount = 1;    //low number loop counter
	int frameDelay;
};

#endif /* USER_APPLICATIONS_WINDOWLIGHTS_METEORSHOWER_H_ */
