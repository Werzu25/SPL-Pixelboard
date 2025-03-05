#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <weatherService.hpp>

// Your credentials and API endpoints
const char *ssid = "iPhone";
const char *password = "kaufdirwlan";
const char *endpoint = "http://api.weatherapi.com/v1/forecast.json";
const char *API_Key = "f666aadff36549bfaf3145737250503";

// Remove the duplicate WeatherData struct definition that was here

WeatherData getWeatherData() {
    WeatherData data = {0.0, "", ""};  // Default values
    
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        String url = String(endpoint) + "?key=" + API_Key +
                     "&q=Innsbruck&days=0&aqi=no&alerts=no";

        http.begin(url);
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println("JSON: " + String(payload));
            StaticJsonDocument<4096> doc; // Using StaticJsonDocument with capacity as template parameter
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

void setupWeather() {
    Serial.begin(115200);

    // Initialize WiFi connection
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");

    // Wait for connection with timeout
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        Serial.print(".");
        timeout++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi connected!");
        Serial.println("IP address: " + WiFi.localIP().toString());
    } else {
        Serial.println();
        Serial.println("Failed to connect to WiFi");
    }
}