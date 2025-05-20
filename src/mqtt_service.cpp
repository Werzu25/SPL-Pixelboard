#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "mqtt_service.hpp"

mqtt_service::mqtt_service(const char* user,
                                   const char* pass,
                                   int port,
                                   PubSubClient client)
    : user(user), password(pass), port(port), client(client) {}


void mqtt_service::connect(void callbackfunction(char*, byte*, unsigned int)) {
    client.setServer(MQTT_Host, port);
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("1", user, password)) {
          Serial.println("Connectet");
        } else {
          Serial.print("Error: ");
          Serial.println(client.state());
        }
      }
    client.setCallback(callbackfunction);
}

void mqtt_service::subscribe(const char* topic) {
    if (client.subscribe(topic)) {
        Serial.println("Subscribed to topic");
    } else {
        Serial.println("Failed to subscribe to topic");
    }
}