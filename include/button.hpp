
class EntprellterTaster {

  public:
    EntprellterTaster(int pin);
    void aktualisieren();
    bool istGedrueckt() const;
    bool wurdeGedrueckt();
    bool wurdeLangGedrueckt();

#ifdef UNIT_TEST

    void simulatePress();

    void simulateRelease();

    void simulateLongPress(unsigned long duration);

#endif

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
