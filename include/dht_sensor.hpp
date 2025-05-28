#ifndef DHT_SENSOR_HPP
#define DHT_SENSOR_HPP
#include <DHT.h>

class dht_sensor
{
private:
    int dhtPin;
    int dhtType;
    DHT *dht;

public:
    dht_sensor (int pin, int type);
    float getTemperature();
    float getHumidity();
};
#endif