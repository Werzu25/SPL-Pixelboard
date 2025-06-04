#include "mqtt.hpp"
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

MqttManager::MqttManager(const char *user, const char *pass, const int port,
                         const char *host)
    : user(user), password(pass), port(port), host(host) {
    static WiFiClientSecure secureClient;
    static PubSubClient mqttClient(secureClient);
    secureClient.setInsecure();
    client = secureClient;
}

void MqttManager::connect(void callbackfunction(char *, byte *, unsigned int)) {
    client.setServer(host, port);
    Serial.println("[MqttManager] Connecting to MQTT Server...");
    while (!client.connected()) {
        if (client.connect("1", user, password)) {
            Serial.println("[MqttManager] Connected");
        } else {
            Serial.print("[MqttManager] Error: ");
            Serial.println(client.state());
        }
        delay(500);
    }
    client.setCallback(callbackfunction);
}

void MqttManager::subscribe(const char *topic) {
    if (client.subscribe(topic)) {
        Serial.print("[MqttManager] Subscribed to topic ");
        Serial.println(topic);
    } else {
        Serial.print("[MqttManager] Failed to subscribe to topic ");
        Serial.println(topic);
    }
}
