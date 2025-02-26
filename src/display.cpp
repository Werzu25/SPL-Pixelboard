#include "../include/display.hpp"
#include "FastLED.h"

int getLed(int x, int y) {
    int res = -1;

    if (y < 8) {
        // 1 LED Matrix

        res = 255 - x * 8;

        if (x % 2 == 0) {
            res = res + y;
        } else {
            res = res - (7 - y);
        }
    } else {
        // 2 LED Matrix
    }

    return res;
}

void setLed(int x, int y, CRGB color, CRGB leds[]) {
    // kann sein dass nit geht weil arduino immer kommisch mit libs
    int index = getLed(x, y);
    leds[index] = color;
    Serial.print("setLed: ");
    Serial.println(index);
    FastLED.show();
}
