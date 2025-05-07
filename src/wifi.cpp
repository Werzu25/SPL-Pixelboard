#include "wifi.hpp"
#include <WiFi.h>

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : ssid(ssid), password(password) {}

void WiFiManager::begin() { WiFi.begin(ssid, password); }

bool WiFiManager::isConnected() { return WiFi.status() == WL_CONNECTED; }

void WiFiManager::reconnect() {
    if (!isConnected()) {
        WiFi.disconnect();
        WiFi.begin(ssid, password);
    }
}
