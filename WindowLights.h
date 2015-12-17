#ifndef __LIGHTSTYPES_H__
#define __LIGHTSTYPES_H__

#include <vector>
#include <map>
#include "FastLED-Sparkcore/firmware/FastLED.h"

#define SWITCH_PIN     D4
#define LED_STRIP_1    D0
#define LED_STRIP_2    D1
#define LED_STRIP_3    D2
#define LED_STRIP_4    D3

#define LEDS_PER_STRIP  114
#define NUM_STRIPS      4
#define NUM_LEDS        (LEDS_PER_STRIP * NUM_STRIPS)
#define NUM_ACTIVE      20
#define NORMAL_BRIGHT   80
#define NUM_COLORS      6
#define NON_PIXEL       -1

#define CHRISTMAS      100
#define HALLOWEEN      101
#define THANKSGIVING   102
#define VALENTINES     103
#define INDEPENDENCE   105
#define NORAH_BDAY		106
#define MADDIE_BDAY		107
#define METEOR_SHOWER	108
#define NEWYEARS		109
#define SNOW			110
#define NO_PROGRAM      999

#define TWINKLE_SPEED 5

// Overall twinkle density.
// 0 (NONE lit) to 8 (ALL lit at once).
// Default is 5.
#define TWINKLE_DENSITY 5

// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.

// How often to change color palettes.
#define SECONDS_PER_PALETTE  20

#define CST_OFFSET      -6
#define DST_OFFSET      (CST_OFFSET + 1)
#define TIME_BASE_YEAR      2015

#define LATITUDE        42.058102
#define LONGITUDE       87.984189

#define APP_VERSION		"2.0"

using namespace NSFastLED;

extern CRGB strip[NUM_STRIPS][LEDS_PER_STRIP];

void isrService();

#endif
