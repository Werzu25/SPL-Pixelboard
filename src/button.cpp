#include <Arduino.h>
#include <button.hpp>

class EntprellterTaster {
private:
    bool tasterZustand = HIGH;
    bool letzterTasterZustand = HIGH;
    unsigned long letzteEntprellZeit = 0;
    unsigned long druckStartZeit = 0;
    unsigned long druckEndeZeit = 0;
    unsigned long letzterKlickZeit = 0;

    const unsigned long entprellVerzoegerung = 50;    // Entprellzeit in Millisekunden
    const unsigned long langDruckDauer = 1000;        // Dauer für langen Druck in Millisekunden
    const unsigned long doppelKlickIntervall = 500;   // Maximales Intervall zwischen Klicks für Doppelklick

    int tasterPin;

    bool kurzGedrueckt = false;
    bool langGedrueckt = false;
    bool doppelGedrueckt = false;

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
                    // Taster gedrückt
                    druckStartZeit = millis();
                } else {
                    // Taster losgelassen
                    druckEndeZeit = millis();
                    unsigned long druckDauer = druckEndeZeit - druckStartZeit;

                    if (druckDauer >= langDruckDauer) {
                        langGedrueckt = true;
                    } else {
                        if ((druckStartZeit - letzterKlickZeit) <= doppelKlickIntervall) {
                            doppelGedrueckt = true;
                            letzterKlickZeit = 0; // Zurücksetzen, um Mehrfachauslösung zu verhindern
                        } else {
                            kurzGedrueckt = true;
                            letzterKlickZeit = druckEndeZeit;
                        }
                    }
                }
            }
        }

        letzterTasterZustand = aktuellerZustand;
    }

    bool wurdeKurzGedrueckt() {
        if (kurzGedrueckt) {
            kurzGedrueckt = false;
            return true;
        }
        return false;
    }

    bool wurdeLangGedrueckt() {
        if (langGedrueckt) {
            langGedrueckt = false;
            return true;
        }
        return false;
    }

    bool wurdeDoppelGedrueckt() {
        if (doppelGedrueckt) {
            doppelGedrueckt = false;
            return true;
        }
        return false;
    }
};
