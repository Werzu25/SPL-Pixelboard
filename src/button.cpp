#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>
#include <button.hpp>

class EntprellterTaster {
private:
    bool tasterZustand = HIGH;
    bool letzterTasterZustand = HIGH;
    bool letzterDruckZustand = false;
    unsigned long druckStartZeit = 0;
    unsigned long letzteEntprellZeit = 0;
    bool wurdeLangGedruecktFlag = false;
    bool wurdeGedruecktFlag = false;
    const unsigned long entprellVerzoegerung = 50;   // Entprellzeit in Millisekunden
    const unsigned long langDruckDauer = 1000;       // Dauer für langen Druck in Millisekunden
    int tasterPin;

public:
    // Konstruktor
    EntprellterTaster(int pin) {
        tasterPin = pin;
        pinMode(tasterPin, INPUT_PULLUP);
    }

    // Aktualisiert den Tasterzustand und entprellt den Taster
    void aktualisieren() {
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

    // Prüft, ob der Taster gedrückt ist
    bool istGedrueckt() const {
        return tasterZustand == LOW;
    }

    // Prüft, ob der Taster gerade gedrückt wurde
    bool wurdeGedrueckt() {
        if (wurdeGedruecktFlag) {
            wurdeGedruecktFlag = false;
            return true;
        }
        return false;
    }

    // Prüft, ob der Taster lange gedrückt wurde
    bool wurdeLangGedrueckt() {
        if (wurdeLangGedruecktFlag) {
            wurdeLangGedruecktFlag = false;
            return true;
        }
        return false;
    }

    // Prüft, ob der Taster lange gedrückt wird
    bool istLangGedrueckt() {
        return (tasterZustand == LOW) && ((millis() - druckStartZeit) >= langDruckDauer);
    }
};

#endif // BUTTON_HPP
