#include "weather.hpp"
#include "HardwareSerial.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

const char *endpoint = "http://api.weatherapi.com/v1/forecast.json";
const char *API_Key = "f666aadff36549bfaf3145737250503";

String Weather::getWeather() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        String url = String(endpoint) + "?key=" + API_Key +
                     "&q=Innsbruck&days=0&aqi=no&alerts=no";

        http.begin(url);
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (error) {
                Serial.print(F("[Weather] deserializeJson() failed: "));
                Serial.println(error.f_str());
                return "";
            } else {
                String location = doc["location"]["name"];
                float temp_c = doc["current"]["temp_c"];
                String condition = doc["current"]["condition"]["text"];

                return "Location: " + location +
                       ", Temperature: " + String(temp_c) +
                       "°C, Condition: " + condition;
            }
        } else {
            Serial.println("[Weather] Error on HTTP request: " +
                           String(httpCode));
            return "";
        }

        http.end();
    } else {
        Serial.println("[Weather] WiFi not connected");
        return "";
    }
}

void Weather::printWeather() {
    Serial.print("[Weather] ");
    Serial.println(getWeather());
}
