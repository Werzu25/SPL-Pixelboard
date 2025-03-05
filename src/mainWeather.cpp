#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "iPhone";
const char* password = "kaufdirwlan";

// Weather API endpoint
const char* endpoint = "https://api.weatherapi.com/v1/forecast.json";
const char* API_Key = "39b152fc4fd84a78a82155410252602";

void setup() {
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

void loop() {
    // Make API call and print response
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        // Create URL with API key and parameters
        String url = String(endpoint) + "?key=" + API_Key + "&q=Innsbruck&days=0&aqi=no&alerts=no";
        
        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println("HTTP Response code: " + String(httpCode));
            Serial.println(payload);
        } else {
            Serial.println("Error on HTTP request: " + String(httpCode));
        }
        
        http.end();
    } else {
        Serial.println("WiFi not connected");
        
        // Attempt to connect to WiFi if not connected
        WiFi.begin(ssid, password);
        Serial.print("Connecting to WiFi");
        
        // Wait for connection with timeout
        int timeout = 0;
        while (WiFi.status() != WL_CONNECTED && timeout < 20) {
            delay(500);
            Serial.print(".");
            timeout++;
        }
        Serial.println();
    }

    delay(60000); // Wait for 1 minute before making the next API call
}