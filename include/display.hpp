#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "crgb.h"
#include <Arduino.h>
#include <FastLED.h>

class Display {
  public:
    Display(int leds1_pin, int leds2_pin);
    void setLed(int x, int y, CRGB color);
    void clear();
    void show();
    void print(String s, int y = 4, int x = 0);
    void rectangle(int w, int h, CRGB color, bool fill = true, int x = 0,
                   int y = 0);

  private:
    int getLed(int x, int y);
    int leds1_pin;
    int leds2_pin;
    CRGB *leds1;
    CRGB *leds2;
};

#endif
