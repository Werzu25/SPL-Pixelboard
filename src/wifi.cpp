#include "wifi.hpp"
#include <WiFi.h>

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : ssid(ssid), password(password) {}

void WiFiManager::begin() {
    Serial.println("[WiFiManager] Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("[WifiManager] WiFi connected");
}

bool WiFiManager::isConnected() { return WiFi.status() == WL_CONNECTED; }

void WiFiManager::reconnect() {
    if (!isConnected()) {
        WiFi.disconnect();
        WiFi.begin(ssid, password);
    }
}
