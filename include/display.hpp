#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <FastLED.h>
#include <Arduino.h>

class Display {
  public:
    Display(int leds1_pin, int leds2_pin);
    void setLed(int x, int y, CRGB color);
    void clear();
    void print(String s);
    void drawRect(int x, int y, int width, int height, CRGB color);

  private:
    int getLed(int x, int y);
    int leds1_pin;
    int leds2_pin;
    CRGB *leds1;
    CRGB *leds2;
};

#endif
