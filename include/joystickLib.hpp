#ifndef JOYSTICK_TASTER_H
#define JOYSTICK_TASTER_H

#include <Arduino.h>

class EntprellterTaster {
private:
    int pin;
    bool entprellterZustand;
    unsigned long letzteAenderung;
    unsigned long druckBeginn;
    bool gedruecktGemeldet;
    static const unsigned long entprellZeit = 50; // 50 ms Entprellzeit
    static const unsigned long langeDruckZeit = 1000; // 1 Sekunde für langen Druck

public:
    EntprellterTaster(int tasterPin);
    void aktualisiere();
    bool istGedrueckt();
    bool wurdeGedrueckt();
    bool wurdeLangeGedrueckt();
};

class Joystick : public EntprellterTaster {
private:
    int xPin, yPin;
    int xWert, yWert;

public:
    Joystick(int tasterPin, int xPin, int yPin);
    void aktualisiere(); // Überschreiben der Methode
    int getX();
    int getY();
};

#endif // JOYSTICK_TASTER_H
