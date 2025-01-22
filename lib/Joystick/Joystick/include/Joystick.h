#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include "Taster.h"

class Joystick : public Taster {
private:
    uint8_t pinX;
    uint8_t pinY;
    int x;
    int y;

public:
    Joystick(uint8_t tasterPin, uint8_t xPin, uint8_t yPin);
    void aktualisiere();
    int getX();
    int getY();
};

#endif // JOYSTICK_H