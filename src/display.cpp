#include "display.hpp"
#include "font.hpp"
#include <FastLED.h>

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
    clear();
    show();
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

void Display::setLed(int x, int y, CRGB color, bool setShow) {
    if (y < 8) {
        leds1[getLed(31 - x, 7 - y)] = color;
    } else {
        leds2[getLed(x, y - 8)] = color;
    }
    if (setShow) {
        show();
    }
}

void Display::clear() { FastLED.clear(true); }

void Display::show() { FastLED.show(); }

void Display::print(String s, int y, int x) {
    clear();

    y = y;
    x = x;

    for (int i = 0; i < s.length(); i++) {
        const uint8_t *charData = getCharData(s[i]);

        for (int col = 0; col < 5; col++) {
            for (int row = 0; row < 7; row++) {
                if (charData[col] & (1 << (6 - row))) {
                    setLed(x + col, y + row, CRGB::White, false);
                }
            }
        }

        x += 6;

        if (x >= 32)
            break;
    }

    show();
}

void Display::rectangle(int w, int h, CRGB color, bool fill, int x, int y) {
    if ((x < 0) || (y < 0) || (x + w > 32) || (y + h > 16)) {
        return;
    }

    if (fill) {
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                setLed(x + i, y + j, color, false);
            }
        }
    } else {
        for (int i = 0; i < w; i++) {
            setLed(x + i, y, color, false);
            setLed(x + i, y + h - 1, color, false);
        }
        for (int j = 0; j < h; j++) {
            setLed(x, y + j, color, false);
            setLed(x + w - 1, y + j, color, false);
        }
    }

    show();
}
