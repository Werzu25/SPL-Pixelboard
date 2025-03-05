#include "joystick.hpp"
#include <Arduino.h>

// Konstruktor
Joystick::Joystick(int buttonPin, int xPin, int yPin) 
    : EntprellterTaster(buttonPin), analogPinX(xPin), analogPinY(yPin), currentX(0), currentY(0) {
    // Keine zusätzliche Initialisierung erforderlich
}

void Joystick::aktualisiere() {
    EntprellterTaster::aktualisiere(); // Aktualisiert den Tasterzustand

    // Lesen der Joystick-Achsen
    currentX = analogRead(analogPinX);
    currentY = analogRead(analogPinY);
}

int Joystick::getX() const {
    return currentX;
}

int Joystick::getY() const {
    return currentY;
}