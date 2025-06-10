#include "pixelboard.hpp"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "joystick.hpp"
#include "mqtt.hpp"
#include <vector>

PixelBoard::PixelBoard(int leds1_pin, int leds2_pin, int joystick_pin,
                       int joystickX_pin, int joystickY_pin, const char *ssid,
                       const char *password, vector<TaskHandle_t> tasks,
                       vector<bool> wasSuspended, const char *mqtt_user,
                       const char *mqtt_password, int mqtt_port,
                       const char *mqtt_host, int dht_pin, int dht_type,
                       const char *project_id, const char *client_email,
                       const char *private_key, const char *spreadsheet_id)
    : display(leds1_pin, leds2_pin),
      joystick(joystick_pin, joystickX_pin, joystickY_pin),
      wifi(ssid, password), tasks(tasks), wasSuspended(wasSuspended),
      mqtt(mqtt_user, mqtt_password, mqtt_port, mqtt_host),
      dht(dht_pin, dht_type),
      spreadsheetWriter(project_id, client_email, private_key, spreadsheet_id) {
    wifi.begin();

    ntp.begin();
    ntp.printLocalTime();

    mqtt.connect(onCallback);
    mqtt.subscribe("snake/input_direction");

    weather.printWeather();
    dht.printTemperature();
    dht.printHumidity();

    GSheet.begin(client_email, project_id, private_key);
}

Direction mqttDirectionTmp = NONE;
String mqttMessageTmp = "";

void onCallback(char *topic, byte *payload, unsigned int length) {
    Serial.printf("[Pixelboard] Message arrived [%s]: ", topic);
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.println(message);

    if (String(topic) == "snake/input_direction") {
        if (message == "UP") {
            mqttDirectionTmp = UP;
        } else if (message == "DOWN") {
            mqttDirectionTmp = DOWN;
        } else if (message == "LEFT") {
            mqttDirectionTmp = LEFT;
        } else if (message == "RIGHT") {
            mqttDirectionTmp = RIGHT;
        } else {
            mqttDirectionTmp = NONE;
            mqttMessageTmp = message;
        }
    }
}

void PixelBoard::updateMqtt() {
    mqttDirection = mqttDirectionTmp;
    mqttMessage = mqttMessageTmp;
    mqttMessageTmp = "";
}
