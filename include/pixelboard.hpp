#ifndef PIXELBOARD_HPP
#define PIXELBOARD_HPP

#include "display.hpp"
#include "joystick.hpp"
#include "wifi.hpp"
#include "mqtt_service.hpp"
#include <PubSubClient.h>
#include "dht_sensor.hpp"
#include <vector>

using namespace std;

class PixelBoard
{
public:
  PixelBoard(int leds1_pin,
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
             int dht_type);

  Display display;
  Joystick joystick;
  WiFiManager wifi;
  mqtt_service mqtt;
  dht_sensor dht_sensor;
  vector<TaskHandle_t> tasks;
  vector<bool> wasSuspended;
  vector<TaskHandle_t> getTasks();
  vector<bool> getWasSuspended();
  void setWasSuspended(vector<bool> v);

private:
  int leds1_pin;
  int leds2_pin;
  int joystick_pin;
  const char *ssid;
  const char *password;
};

#endif