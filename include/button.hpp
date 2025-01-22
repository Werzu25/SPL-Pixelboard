#ifndef BUTTON_HPP
#define BUTTON_HPP

class EntprellterTaster {

  public:
    EntprellterTaster(int pin);
    void aktualisieren();
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

#endif
