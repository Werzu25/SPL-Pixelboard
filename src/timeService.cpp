#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "timeService.hpp"

// NTP Server settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;     // Set your timezone offset in seconds (e.g., 3600 for GMT+1)
const int   daylightOffset_sec = 3600; // Set your daylight saving time offset in seconds (typically 3600)

bool TimeService::begin() {
    // Wait for WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected - cannot sync time");
        return false;
    }
    
    // Configure NTP time synchronization
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    // Wait until time is synchronized
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
    printLocalTime();
    return true;
}

void TimeService::printLocalTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

bool TimeService::updateTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return false;
    }
    return true;
}

time_t TimeService::getCurrentTime() {
    time_t now;
    time(&now);
    return now;
}

struct tm TimeService::getCurrentTimeStruct() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo;
}