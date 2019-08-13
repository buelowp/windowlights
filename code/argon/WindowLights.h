#ifndef __LIGHTSTYPES_H__
#define __LIGHTSTYPES_H__

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

#define CHRISTMAS      		100
#define HALLOWEEN      		101
#define THANKSGIVING   		102
#define VALENTINES     		103
#define INDEPENDENCE   		105
#define NORAH_BDAY			106
#define MADDIE_BDAY			107
#define METEOR_SHOWER		108
#define NEWYEARS			109
#define SNOW				110
#define NEW_YEARS			111
#define NO_PROGRAM      	999

#define ONE_HOUR            (1000 * 60 * 60 * 1)
#define SIX_HOURS           (1000 * 60 * 60 * 6)

// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.

#define CST_OFFSET      	-6
#define DST_OFFSET      	(CST_OFFSET + 1)
#define TIME_BASE_YEAR      2015

#define LATITUDE        	42.058102
#define LONGITUDE       	87.984189

#endif
