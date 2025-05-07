#include <FastLED.h>
#include <stdlib.h>
#include <FontMatrise.h>
#include "display.hpp"
#include "ntp.hpp"

#define NUM_LEDS 256

Display::Display(int leds1_pin, int leds2_pin)
    : leds1_pin(leds1_pin), leds2_pin(leds2_pin) {
    leds1 = new CRGB[NUM_LEDS];
    leds2 = new CRGB[NUM_LEDS];

    switch (leds1_pin) {
    case 25:
        FastLED.addLeds<WS2812B, 25, GRB>(leds1, NUM_LEDS);
        break;
    default:
        break;
    }
    switch (leds2_pin) {
    case 26:
        FastLED.addLeds<WS2812B, 26, GRB>(leds2, NUM_LEDS);
        break;
    default:
        break;
    }

    FastLED.setBrightness(50);
    FastLED.clear(true);
    FastLED.show();
}

int Display::getLed(int x, int y) {
    int res = x * 8;

    if (x % 2 == 0) {
        res = res + 7 - y;
    } else {
        res = res + y;
    }

    return res;
}

void Display::setLed(int x, int y, CRGB color) {
    if (y < 8) {
        leds1[getLed(31 - x, 7 - y)] = color;
    } else {
        leds2[getLed(x, y - 8)] = color;
    }
    FastLED.show();
}

void Display::clear() { FastLED.clear(true); }


void Display::print(char* str) {
    auto tm = ntp.getCurrentTimeStruct();
    tbuff[0] = EFFECT_SCROLL_LEFT[0];
    int len = snprintf((char*)(tbuff + 1),
                       sizeof(tbuff) - 1,str);
    ScrollingMsg.SetText(tbuff, (unsigned char)(len + 1));
    if (ScrollingMsg.UpdateText() == -1) {
      ScrollingMsg.SetText(tbuff, (unsigned char)(len + 1));
    }
    FastLED.show();
    vTaskDelay(pdMS_TO_TICKS(SCROLL_SPEED));
      
}