#ifndef PIXELBOARD_HPP
#define PIXELBOARD_HPP

#include "display.hpp"
#include "joystick.hpp"
#include "wifi.hpp"
#include "mqtt_service.hpp"
#include <PubSubClient.h>
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
             const char *mqtt_user,
             const char *mqtt_password,
             int mqtt_port,
             PubSubClient mqtt_client);

  Display display;
  Joystick joystick;
  WiFiManager wifi;
  mqtt_service mqtt;
  vector<TaskHandle_t> tasks;
  vector<TaskHandle_t> getTasks();

private:
  int leds1_pin;
  int leds2_pin;
  int joystick_pin;
  const char *ssid;
  const char *password;
};

#endif