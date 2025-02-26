#include <Arduino.h>
#include "button.hpp"
#include "joystick.h"

EntprellterTaster taster(32);  // Initialisiert den Taster an Pin 32
Joystick joystick(25, 34, 35); // Initialisiert den Joystick an den Pins 25, 34 und 35

void setup() {
    Serial.begin(115200);
}

void loop() {
    taster.aktualisieren();
    joystick.aktualisieren();

    if (taster.wurdeGedrueckt()) {
        Serial.println("Kurzer Tastendruck erkannt");
    }

    if (taster.wurdeLangGedrueckt()) {
        Serial.println("Langer Tastendruck erkannt");
    }

    int x = joystick.getX();
    int y = joystick.getY();
    bool joystickButtonPressed = joystick.istGedrueckt();

    Serial.print("Joystick X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.println(y);
    Serial.print("Joystick Button ist ");
    Serial.println(joystickButtonPressed ? "GEDRUECKT" : "NICHT GEDRUECKT");

    delay(500);  // Verzögerung von 500ms
}
