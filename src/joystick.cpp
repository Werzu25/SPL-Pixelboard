#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <Arduino.h>
#include "button.h"

class Joystick : public EntprellterTaster {
private:
    int pinX;
    int pinY;

public:
    // Constructor
    Joystick(int pinButton, int pinX, int pinY) : EntprellterTaster(pinButton), pinX(pinX), pinY(pinY) {
        pinMode(pinX, INPUT);
        pinMode(pinY, INPUT);
    }

    // Get the X position of the joystick
    int getX() {
        return analogRead(pinX);
    }

    // Get the Y position of the joystick
    int getY() {
        return analogRead(pinY);
    }
};

#endif // JOYSTICK_HPP