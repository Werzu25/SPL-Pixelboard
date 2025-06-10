#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <Arduino.h>

class Weather {
  public:
    String getWeather();
    void printWeather();
};

#endif
