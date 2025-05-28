#include "dht_sensor.hpp"
#include <DHT.h>
#include <Arduino.h>


dht_sensor::dht_sensor(int pin, int type) : dhtPin(pin), dhtType(type) {
    dht = new DHT(dhtPin, dhtType);
    dht->begin();
}


float dht_sensor::getTemperature() {
    return dht->readTemperature();
}

float dht_sensor::getHumidity() {
    return dht->readHumidity();
}
