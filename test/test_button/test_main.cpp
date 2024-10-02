#include <Arduino.h>

int schalterInput = 32;

void setup() {
  pinMode(schalterInput, INPUT_PULLUP);  // Enable internal pull-up resistor
  Serial.begin(115200);  
}

void loop() {
  int schalter = digitalRead(schalterInput);
  Serial.println(schalter);
  delay(1000);
}
