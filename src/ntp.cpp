#include "ntp.hpp"
#include "HardwareSerial.h"
#include <Arduino.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void Ntp(void *pvParameters) {
    vTaskDelay(50);
    begin();
    updateTime();

    while (1) {
        
        vTaskDelay(10);
    };
}


#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "ntp.hpp"

bool begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected - cannot sync time");
        return false;
    }
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Waiting for NTP time sync...");
    
    time_t now = 0;
    struct tm timeinfo;
    int retry = 0;
    const int maxRetries = 10;
    
    while (!getLocalTime(&timeinfo) && retry < maxRetries) {
        Serial.println("Failed to obtain time, retrying...");
        delay(1000);
        retry++;
    }
    
    if (retry >= maxRetries) {
        Serial.println("Failed to sync time after maximum retries");
        return false;
    }
    
    Serial.println("Time synchronized successfully");
    return true;
}

bool updateTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return false;
    }
    return true;
}

time_t getCurrentTime() {
    time_t now;
    time(&now);
    return now;
}

struct tm getCurrentTimeStruct() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo;
}