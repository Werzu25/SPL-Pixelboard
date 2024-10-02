#include <Arduino.h>
int schalter_input =32;
void setup() {
  pinMode(32, INPUT);

  
}
void loop() {
  int schalter = digitalRead(schalter_input);
  Serial.println(schalter);
  delay(1000);
}