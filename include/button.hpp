#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

class EntprellterTaster {
private:
    uint8_t pin;
    unsigned long letzteEntprellZeit;
    unsigned long entprellVerzoegerung;
    bool letzterTasterZustand;
    bool tasterZustand;
    bool letzterDruckZustand;

public:
    EntprellterTaster(uint8_t tasterPin, unsigned long entprellVerzoegerungMs = 50);

    void aktualisieren();
    bool istLangGedrueckt(unsigned long langDruckDauer);
    bool istGedrueckt() const;
    bool wurdeGedrueckt();
};

#endif
