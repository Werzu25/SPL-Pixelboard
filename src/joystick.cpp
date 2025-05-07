#include "joystick.hpp"
#include <Arduino.h>

Joystick::Joystick(int buttonPin, int xPin, int yPin)
    : Button(buttonPin), analogPinX(xPin), analogPinY(yPin), currentX(0),
      currentY(0), currentDirection(NONE) {}

void Joystick::update() {
    Button::update();

    currentX = analogRead(analogPinX);
    currentY = analogRead(analogPinY);

    if (currentX > 2500) {
        if (currentDirection != UP)
            currentDirection = DOWN;
    } else if (currentX < 1500) {
        if (currentDirection != DOWN)
            currentDirection = UP;
    } else if (currentY > 2500) {
        if (currentDirection != RIGHT)
            currentDirection = LEFT;
    } else if (currentY < 1500) {
        if (currentDirection != LEFT)
            currentDirection = RIGHT;
    } else {
        currentDirection = NONE;
    }
}

int Joystick::getX() const { return currentX; }

int Joystick::getY() const { return currentY; }

Direction Joystick::getCurrentDirection() const { return currentDirection; }
