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

void Display::print(String s) {
    clear();
    int x = 0;
    int y = 4; // Center vertically in the 16-pixel height display
    
    for (int i = 0; i < s.length(); i++) {
        const uint8_t* charData = getCharData(s[i]);
        
        // Draw each column of the character
        for (int col = 0; col < 5; col++) {
            if (x + col >= 32) break; // Don't draw beyond display width
            
            // Draw each pixel in the column
            for (int row = 0; row < 7; row++) {
                if (charData[row] & (0x10 >> col)) { // Check if pixel should be lit
                    setLed(x + col, y + row, CRGB::White);
                }
            }
        }
        
        x += 6; // Move to next character position (5 pixels + 1 pixel spacing)
        if (x >= 32) break; // Stop if we've reached the end of the display
    }
}

void Display::drawRect(int x, int y, int width, int height, CRGB color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            setLed(x + i, y + j, color);
        }
    }
}