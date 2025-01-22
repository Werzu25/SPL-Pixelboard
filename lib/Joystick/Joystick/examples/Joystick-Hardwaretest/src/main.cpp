#include <Arduino.h>
#include <Joystick.h>

// Beispiel-Pins
#define switchPin 25
#define pinX 34
#define pinY 35

Joystick joystick(switchPin, pinX, pinY); // Joystick-Objekt erstellen

void setup() {
    Serial.begin(115200); // Serielle Kommunikation starten
}

void loop() {
    joystick.aktualisiere();
    int x = joystick.getX();
    int y = joystick.getY();
    bool tasterPressed = joystick.istGedrueckt();

    Serial.print("Taster ist ");
    Serial.println(tasterPressed ? "GEDRUECKT" : "NICHT GEDRUECKT");
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.println(y);
    delay(500);
}