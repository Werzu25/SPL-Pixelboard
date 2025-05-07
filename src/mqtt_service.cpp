#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "mqtt_service.hpp"

mqtt_service::mqtt_service(const char* user,
                                   const char* pass,
                                   int port,
                                   PubSubClient client)
    : user(user), password(pass), port(port), client(client) {}

char* callback(char* topic, byte* payload, unsigned int length) {
    char* payloadString = (char*)malloc(length + 1);
    return payloadString;
}

void mqtt_service::connect() {
    client.setServer(MQTT_Host, port);
    client.setCallback(callback);
}

void mqtt_service::subscribe(const char* topic) {
    if (client.subscribe(topic)) {
        Serial.println("Subscribed to topic");
    } else {
        Serial.println("Failed to subscribe to topic");
    }
}

