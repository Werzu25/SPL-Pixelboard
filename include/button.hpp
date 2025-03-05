#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

class EntprellterTaster {
public:
    EntprellterTaster(int pin);
    virtual void aktualisiere(); // Declare as virtual
    bool istGedrueckt() const;
    bool wurdeGedrueckt();
    bool wurdeLangGedrueckt();

private:
    int tasterPin;
    bool tasterZustand;
    bool letzterTasterZustand;
    bool letzterDruckZustand;
    unsigned long druckStartZeit;
    unsigned long letzteEntprellZeit;
    bool langGedruecktErkannt;
    bool wurdeGedruecktFlag;
    bool wurdeLangGedruecktFlag;
    static const unsigned long entprellVerzoegerung = 50;
    static const unsigned long langDruckDauer = 1000;
};

#endif // BUTTON_HPP