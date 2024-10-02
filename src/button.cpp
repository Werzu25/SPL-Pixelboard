#include <Arduino.h>

// Die Klasse EntprellterTaster verwaltet die Entprellung eines Tasters.
class EntprellterTaster {
private:
    uint8_t pin;
    unsigned long letzteEntprellZeit;
    unsigned long entprellVerzögerung;
    bool letzterTasterZustand;
    bool tasterZustand;
    bool letzterDruckZustand;

public:
    // Konstruktor für die Klasse EntprellterTaster
    EntprellterTaster(uint8_t tasterPin, unsigned long entprellVerzögerungMs = 50)
        : pin(tasterPin), entprellVerzögerung(entprellVerzögerungMs), letzteEntprellZeit(0), letzterTasterZustand(HIGH), tasterZustand(HIGH), letzterDruckZustand(false) {
        pinMode(pin, INPUT_PULLUP);
    }

    // Diese Methode aktualisiert den Tasterzustand und entprellt ihn.
    void aktualisieren() {
        bool aktuellerZustand = digitalRead(pin);
        if (aktuellerZustand != letzterTasterZustand) {
            letzteEntprellZeit = millis();
        }

        if ((millis() - letzteEntprellZeit) > entprellVerzögerung) {
            if (aktuellerZustand != tasterZustand) {
                tasterZustand = aktuellerZustand;
            }
        }

        letzterTasterZustand = aktuellerZustand;
    }

    // Diese Methode prüft, ob der Taster gedrückt ist.
    bool istGedrueckt() const {
        return !tasterZustand;
    }

    // Diese Methode prüft, ob der Taster gerade gedrückt wurde.
    bool wurdeGedrueckt() {
        bool wurdeGedrueckt = (tasterZustand == LOW && letzterDruckZustand == false);
        letzterDruckZustand = (tasterZustand == LOW);
        return wurdeGedrueckt;
    }
};