#include "joystickLib.hpp"

// Implementierung der Klasse EntprellterTaster
EntprellterTaster::EntprellterTaster(int tasterPin)
    : pin(tasterPin), entprellterZustand(false), letzteAenderung(0), druckBeginn(0), gedruecktGemeldet(true) {
    pinMode(pin, INPUT_PULLUP);
}

void EntprellterTaster::aktualisiere() {
    bool aktuellerZustand = !digitalRead(pin); // Negation für negative Logik
    unsigned long aktuelleZeit = millis();

    if (aktuellerZustand != entprellterZustand && (aktuelleZeit - letzteAenderung) > entprellZeit) {
        entprellterZustand = aktuellerZustand;
        letzteAenderung = aktuelleZeit;

        if (entprellterZustand) { // steigende Flanke
            druckBeginn = aktuelleZeit;
            gedruecktGemeldet = false;
        }
    }
}

bool EntprellterTaster::istGedrueckt() {
    return entprellterZustand;
}

bool EntprellterTaster::wurdeGedrueckt() {
    if (!entprellterZustand && !gedruecktGemeldet) {
        if (millis() - druckBeginn < langeDruckZeit) {
            gedruecktGemeldet = true;
            return true;
        }
    }
    return false;
}

bool EntprellterTaster::wurdeLangeGedrueckt() {
    if (entprellterZustand && !gedruecktGemeldet) {
        if (millis() - druckBeginn >= langeDruckZeit) {
            gedruecktGemeldet = true;
            return true;
        }
    }
    return false;
}

// Implementierung der Klasse Joystick
Joystick::Joystick(int tasterPin, int xPin, int yPin)
    : EntprellterTaster(tasterPin), xPin(xPin), yPin(yPin), xWert(0), yWert(0) {
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
}

void Joystick::aktualisiere() {
    EntprellterTaster::aktualisiere(); // Aufruf der Basisklassenmethode
    xWert = analogRead(xPin);
    yWert = analogRead(yPin);
}

int Joystick::getX() {
    return xWert;
}

int Joystick::getY() {
    return yWert;
}
