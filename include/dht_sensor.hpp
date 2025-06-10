#ifndef DHT_HPP
#define DHT_HPP

#include <Arduino.h>
#include <DHT.h>

class DhtSensor {
  private:
    int dhtPin;
    int dhtType;
    DHT *dht;

  public:
    DhtSensor(int pin, int type);
    float getTemperature();
    float getHumidity();
    void printHumidity();
    void printTemperature();
};

#endif
