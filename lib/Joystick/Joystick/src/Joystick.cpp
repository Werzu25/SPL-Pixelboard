#include "Joystick.h"

Joystick::Joystick(uint8_t tasterPin, uint8_t xPin, uint8_t yPin) : Taster(tasterPin), pinX(xPin), pinY(yPin), x(0), y(0) {
    pinMode(pinX, INPUT);
    pinMode(pinY, INPUT);
}

void Joystick::aktualisiere() {
    Taster::aktualisiere();
    x = analogRead(pinX);
    y = analogRead(pinY);
}

int Joystick::getX() {
    return x;
}

int Joystick::getY() {
    return y;
}