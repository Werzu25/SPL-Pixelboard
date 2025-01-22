#ifndef TASTER_H
#define TASTER_H

#include <Arduino.h>

class Taster {
private:
    uint8_t pin;
    bool entprellterZustand;
    bool letzterZustand;
    unsigned long letztesUpdate;
    const unsigned long entprellZeit = 50; // Entprellzeit in Millisekunden
    unsigned long gedruecktZeit;
    bool gedrueckt;
    bool langeGedrueckt;

public:
    Taster(uint8_t p);
    void aktualisiere();
    bool istGedrueckt();
    bool wurdeGedrueckt();
    bool wurdeLangeGedrueckt();
};

#endif // TASTER_H