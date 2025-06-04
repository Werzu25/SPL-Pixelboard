#include "dht_sensor.hpp"
#include "HardwareSerial.h"
#include <Arduino.h>
#include <DHT.h>

DhtSensor::DhtSensor(int pin, int type) : dhtPin(pin), dhtType(type) {
    dht = new DHT(dhtPin, dhtType);
    dht->begin();
}

float DhtSensor::getTemperature() { return dht->readTemperature(); }

float DhtSensor::getHumidity() { return dht->readHumidity(); }

void DhtSensor::printTemperature() {
    Serial.print("[Dht] Temperature: ");
    Serial.println(getTemperature());
}

void DhtSensor::printHumidity() {
    Serial.print("[Dht] Humidity: ");
    Serial.println(getHumidity());
}
