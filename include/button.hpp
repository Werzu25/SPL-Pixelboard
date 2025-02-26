#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

class EntprellterTaster {
private:
    int tasterPin;
    bool tasterZustand = HIGH;
    bool letzterTasterZustand = HIGH;
    unsigned long letzteEntprellZeit = 0;
    const unsigned long entprellVerzoegerung = 50;   // Entprellzeit in Millisekunden

    unsigned long druckStartZeit = 0;
    const unsigned long langDruckDauer = 1000;       // Dauer für langen Druck in Millisekunden

    bool wurdeGedruecktFlag = false;
    bool wurdeLangGedruecktFlag = false;
    bool langGedruecktErkannt = false;              // Flag für sofortige Langdruckerkennung

#ifdef UNIT_TEST
    bool simulatedState = HIGH; // Simulierter Zustand für Unit-Tests
#endif

public:
    // Konstruktor
    EntprellterTaster(int pin);

    // Aktualisiert den Tasterzustand und entprellt den Taster
    void aktualisiere();

    // Prüft, ob der Taster gedrückt ist
    bool istGedrueckt() const;

    // Prüft, ob der Taster gerade gedrückt wurde
    bool wurdeGedrueckt();

    // Prüft, ob der Taster lange gedrückt wurde
    bool wurdeLangGedrueckt();

#ifdef UNIT_TEST
    // Simulationsmethoden für Unit-Tests
    void simulatePress();
    void simulateRelease();
    void simulateLongPress(unsigned long duration);
#endif
};

#endif // BUTTON_HPP