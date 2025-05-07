#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <weather.hpp>

const char *endpoint = "http://api.weatherapi.com/v1/forecast.json";
const char *API_Key = "f666aadff36549bfaf3145737250503";

WeatherData getWeatherData() {
    WeatherData data = {0.0, "", ""};
    
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        String url = String(endpoint) + "?key=" + API_Key +
                     "&q=Innsbruck&days=0&aqi=no&alerts=no";

        http.begin(url);
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println("JSON: " + String(payload));
            StaticJsonDocument<4096> doc;
            DeserializationError error = deserializeJson(doc, payload);
            
            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
            } else {
                data.temp_c = doc["current"]["temp_c"];
                data.location = doc["location"]["name"];
                data.condition = doc["current"]["condition"]["text"];
            }
        } else {
            Serial.println("Error on HTTP request: " + String(httpCode));
        }

        http.end();
    }
    return data;
}