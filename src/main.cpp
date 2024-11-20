#include <Arduino.h>
#include "button.hpp"

EntprellterTaster taster(32);  // Initialisiert den Taster an Pin 32

void setup() {
    Serial.begin(115200);
}

void loop() {
    taster.aktualisieren();

    if (taster.wurdeGedrueckt()) {
        Serial.println("Kurzer Tastendruck erkannt");
    }

    if (taster.wurdeLangGedrueckt()) {
        Serial.println("Langer Tastendruck erkannt");
    }

    delay(10);  // Kleine Verzögerung, um den Serial Monitor nicht zu überfluten
}