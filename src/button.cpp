#include <Arduino.h>
#include <button.hpp>

class EntprellterTaster {
private:
    int tasterPin;
    bool tasterZustand = HIGH;
    bool letzterTasterZustand = HIGH;
    unsigned long letzteEntprellZeit = 0;
    const unsigned long entprellVerzoegerung = 50; // Entprellzeit in Millisekunden

    unsigned long druckStartZeit = 0;
    const unsigned long langDruckDauer = 1000;     // Dauer für langen Tastendruck in Millisekunden

    bool wurdeGedruecktFlag = false;
    bool wurdeLangGedruecktFlag = false;

public:
    EntprellterTaster(int pin) {
        tasterPin = pin;
        pinMode(tasterPin, INPUT_PULLUP);
    }

    void aktualisiere() {
        bool aktuellerZustand = digitalRead(tasterPin);

        if (aktuellerZustand != letzterTasterZustand) {
            letzteEntprellZeit = millis();
        }

        if ((millis() - letzteEntprellZeit) > entprellVerzoegerung) {
            if (aktuellerZustand != tasterZustand) {
                tasterZustand = aktuellerZustand;

                if (tasterZustand == LOW) {
                    druckStartZeit = millis();
                } else {
                    unsigned long druckDauer = millis() - druckStartZeit;
                    if (druckDauer >= langDruckDauer) {
                        wurdeLangGedruecktFlag = true;
                    } else {
                        wurdeGedruecktFlag = true;
                    }
                }
            }
        }

        letzterTasterZustand = aktuellerZustand;
    }

    bool istGedrueckt() const {
        return tasterZustand == LOW;
    }

    bool wurdeGedrueckt() {
        if (wurdeGedruecktFlag) {
            wurdeGedruecktFlag = false;
            return true;
        }
        return false;
    }

    bool wurdeLangGedrueckt() {
        if (wurdeLangGedruecktFlag) {
            wurdeLangGedruecktFlag = false;
            return true;
        }
        return false;
    }
};
