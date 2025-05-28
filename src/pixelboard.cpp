#include "pixelboard.hpp"
#include "Arduino.h"

PixelBoard::PixelBoard(int leds1_pin,
                       int leds2_pin,
                       int joystick_pin,
                       int joystickX_pin,
                       int joystickY_pin,
                       const char *ssid,
                       const char *password,
                       const vector<TaskHandle_t> &tasks,
                       const vector<bool> &wasSuspended,
                       const char *mqtt_user,
                       const char *mqtt_password,
                       int mqtt_port,
                       PubSubClient mqtt_client,
                       int dht_pin,
                       int dht_type)
    : display(leds1_pin, leds2_pin),
      joystick(joystick_pin, joystickX_pin, joystickY_pin),
      wifi(ssid, password),
      tasks(tasks),
      wasSuspended(wasSuspended),
      mqtt(mqtt_user, mqtt_password, mqtt_port, mqtt_client),
      dht(dht_pin, dht_type) {
}

vector<TaskHandle_t> PixelBoard::getTasks() {
  return tasks;
}

vector<bool> PixelBoard::getWasSuspended() {
  return wasSuspended;
}

void PixelBoard::setWasSuspended(vector<bool> v) {
  wasSuspended = v;
}