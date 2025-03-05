#pragma once

struct WeatherData {
    float temp_c;
    const char *location;
    const char *condition;
};

// Function declarations
WeatherData getWeatherData();
void setupWeather();