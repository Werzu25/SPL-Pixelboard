#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <FastLED.h>

int getLed(int x, int y);
void setLed(int x, int y, CRGB color, CRGB leds[]);

#endif
