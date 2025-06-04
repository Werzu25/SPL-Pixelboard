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
                       const char *mqtt_host, int dht_pin, int dht_type)
    : display(leds1_pin, leds2_pin),
      joystick(joystick_pin, joystickX_pin, joystickY_pin),
      wifi(ssid, password), tasks(tasks), wasSuspended(wasSuspended),
      mqtt(mqtt_user, mqtt_password, mqtt_port, mqtt_host), dht(dht_pin, dht_type) {
    // wifi.begin();
    //
    // ntp.begin();
    // ntp.printLocalTime();
    //
    // mqtt.connect(onCallback);
    // mqtt.subscribe("snake/input_direction");
    //
    // weather.printWeather();
    // dht.printTemperature();
    // dht.printHumidity();
}

Direction mqttDirectionTmp = NONE;

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
        }
    }
}

void PixelBoard::updateMqttDiretion() { mqttDirection = mqttDirectionTmp; }
