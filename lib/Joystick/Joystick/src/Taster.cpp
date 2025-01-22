#include "Taster.h"

Taster::Taster(uint8_t p) : pin(p), entprellterZustand(false), letzterZustand(false), letztesUpdate(0), gedruecktZeit(0), gedrueckt(false), langeGedrueckt(false) {
    pinMode(pin, INPUT); // Setzt den Pin als Eingang (ohne Pullup)
}

void Taster::aktualisiere() {
    bool aktuellerZustand = digitalRead(pin); // Keine Negation notwendig
    unsigned long jetzt = millis();

    if (aktuellerZustand != letzterZustand) {
        letztesUpdate = jetzt;
    }

    if ((jetzt - letztesUpdate) > entprellZeit) {
        if (aktuellerZustand != entprellterZustand) {
            entprellterZustand = aktuellerZustand;
            if (entprellterZustand) {
                gedruecktZeit = jetzt;
                gedrueckt = true;
                langeGedrueckt = false;
            } else {
                gedrueckt = false;
            }
        }
    }

    letzterZustand = aktuellerZustand;
}

bool Taster::istGedrueckt() {
    return entprellterZustand;
}

bool Taster::wurdeGedrueckt() {
    if (!entprellterZustand && gedrueckt) {
        gedrueckt = false;
        if (millis() - gedruecktZeit < 1000) {
            return true;
        }
    }
    return false;
}

bool Taster::wurdeLangeGedrueckt() {
    if (entprellterZustand && !langeGedrueckt) {
        if (millis() - gedruecktZeit >= 1000) {
            langeGedrueckt = true;
            return true;
        }
    }
    return false;
}