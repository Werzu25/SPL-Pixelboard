#include "../include/display.hpp"
#include "FastLED.h"
int getLed(int x, int y) {
    int width = 8; // Breite jeder Matrixzeile
    int num_matrices = 2; // Anzahl der Matrizen (8x32), angenommen

    // Gesamter Index über mehrere Matrizen hinweg unter Berücksichtigung des Zickzack-Musters
    int base_index = (y / 8) * 256; // Jede Matrix hat 256 LEDs (8x32)

    // Umkehrung des Zickzack-Musters je nach geradem oder ungeradem x
    if ((x % 2) == 0) {
        // Gerade x-Werte: LED-Index nimmt zu
        return base_index + x * 8 + y % 8;
    } else {
        // Ungerade x-Werte: LED-Index nimmt ab
        return base_index + x * 8 + (7 - (y % 8));
    }
}



void setLed(int x, int y, CRGB color, CRGB leds[]) {
    // kann sein dass nit geht weil arduino immer kommisch mit libs
    int index = getLed(x, y);
    leds[index] = color;
    Serial.print("setLed: ");
    Serial.println(index);
    FastLED.show();
}
