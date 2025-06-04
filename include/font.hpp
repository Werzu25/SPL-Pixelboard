#ifndef FONT_HPP
#define FONT_HPP

#include <Arduino.h>

extern const uint8_t font5x7[];

const uint8_t *getCharData(char c);

#endif
