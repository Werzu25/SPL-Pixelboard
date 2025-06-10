#include "ntp.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include "HardwareSerial.h"
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;  
const int   daylightOffset_sec = 3600; 

bool Ntp::begin() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    Serial.println("[Ntp] Waiting for NTP time sync...");
    
    time_t now = 0;
    struct tm timeinfo;
    int retry = 0;
    const int maxRetries = 10;
    
    while (!getLocalTime(&timeinfo) && retry < maxRetries) {
        Serial.println("[Ntp] Failed to obtain time, retrying...");
        delay(1000);
        retry++;
    }
    
    if (retry >= maxRetries) {
        Serial.println("[Ntp] Failed to sync time after maximum retries");
        return false;
    }
    
    Serial.println("[Ntp] Time synchronized successfully");
    return true;
}

void Ntp::printLocalTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("[Ntp] Failed to obtain time");
        return;
    }
    Serial.print("[Ntp] ");
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

bool Ntp::updateTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("[Ntp] Failed to obtain time");
        return false;
    }
    return true;
}

time_t Ntp::getCurrentTime() {
    time_t now;
    time(&now);
    return now;
}

struct tm Ntp::getCurrentTimeStruct() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo;
}
