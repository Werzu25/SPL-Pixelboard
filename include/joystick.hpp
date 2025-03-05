#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include "button.hpp"

class Joystick : public EntprellterTaster {
private:
    int analogPinX;
    int analogPinY;
    int currentX;
    int currentY;

public:
    // Konstruktor
    Joystick(int buttonPin, int xPin, int yPin);

    // Überschreibt die Aktualisierungsmethode, um Joystick-Daten zu lesen
    void aktualisiere() override;
    // Getter für X-Wert des Joysticks
    int getX() const;

    // Getter für Y-Wert des Joysticks
    int getY() const;
};

#endif // JOYSTICK_HPP