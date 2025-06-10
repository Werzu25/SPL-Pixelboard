#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include "button.hpp"

enum Direction { LEFT, RIGHT, UP, DOWN, NONE };

class Joystick : public Button {
  private:
    int analogPinX;
    int analogPinY;
    int currentX;
    int currentY;
    Direction currentDirection;

  public:
    Joystick(int buttonPin, int xPin, int yPin);
    void update() override;
    int getX();
    int getY();
    Direction getCurrentDirection();
    void setCurrentDirection(Direction direction);
};

#endif
