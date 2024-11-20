#include "button.hpp"
#include <Arduino.h>

// Konstruktor
EntprellterTaster::EntprellterTaster(int pin) {
    tasterPin = pin;
    pinMode(tasterPin, INPUT_PULLUP);
    tasterZustand = HIGH;
    letzterTasterZustand = HIGH;
    letzterDruckZustand = false;
    druckStartZeit = 0;
    letzteEntprellZeit = 0;
    langGedruecktErkannt = false;
    wurdeGedruecktFlag = false;
    wurdeLangGedruecktFlag = false;
}

// Aktualisiert den Tasterzustand und entprellt den Taster
void EntprellterTaster::aktualisieren() {
#ifdef UNIT_TEST
    // Unit test specific code
#else
    bool aktuellerZustand = digitalRead(tasterPin);
#endif

    if (aktuellerZustand != letzterTasterZustand) {
        letzteEntprellZeit = millis();
    }

    if ((millis() - letzteEntprellZeit) > entprellVerzoegerung) {
        if (aktuellerZustand != tasterZustand) {
            tasterZustand = aktuellerZustand;

            if (tasterZustand == LOW) {
                druckStartZeit = millis();
                langGedruecktErkannt = false; // Zurücksetzen des Long-Press-Flags
            } else {
                unsigned long druckDauer = millis() - druckStartZeit;
                if (druckDauer >= langDruckDauer && !langGedruecktErkannt) {
                    wurdeLangGedruecktFlag = true;
                    langGedruecktErkannt = true; // Verhindert wiederholte Erkennung
                } else if (druckDauer < langDruckDauer) {
                    wurdeGedruecktFlag = true;
                }
                druckStartZeit = 0;
            }
        }
    }

    // Sofortige Langdruck-Erkennung
    if (tasterZustand == LOW && !langGedruecktErkannt) {
        if ((millis() - druckStartZeit) >= langDruckDauer) {
            wurdeLangGedruecktFlag = true;
            langGedruecktErkannt = true; // Verhindert wiederholte Erkennung
        }
    }

    letzterTasterZustand = aktuellerZustand;
}

bool EntprellterTaster::istGedrueckt() const {
    return tasterZustand == LOW;
}

bool EntprellterTaster::wurdeGedrueckt() {
    if (wurdeGedruecktFlag) {
        wurdeGedruecktFlag = false;
        return true;
    }
    return false;
}

bool EntprellterTaster::wurdeLangGedrueckt() {
    if (wurdeLangGedruecktFlag) {
        wurdeLangGedruecktFlag = false;
        return true;
    }
    return false;
}

#ifdef UNIT_TEST
void EntprellterTaster::simulatePress() {
    tasterZustand = LOW;
}

void EntprellterTaster::simulateRelease() {
    tasterZustand = HIGH;
}

void EntprellterTaster::simulateLongPress(unsigned long duration) {
    druckStartZeit = millis() - duration;
    tasterZustand = LOW;
    langGedruecktErkannt = false;
}
#endif