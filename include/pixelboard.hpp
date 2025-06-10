#ifndef PIXELBOARD_HPP
#define PIXELBOARD_HPP

#include "dht_sensor.hpp"
#include "display.hpp"
#include "joystick.hpp"
#include "mqtt.hpp"
#include "ntp.hpp"
#include "spreadsheet_writer.hpp"
#include "weather.hpp"
#include "wifi.hpp"
#include <vector>

using namespace std;

class PixelBoard {
  public:
    PixelBoard(int leds1_pin, int leds2_pin, int joystick_pin,
               int joystickX_pin, int joystickY_pin, const char *ssid,
               const char *password, vector<TaskHandle_t> tasks,
               vector<bool> wasSuspended, const char *mqtt_user,
               const char *mqtt_password, int mqtt_port, const char *mqtt_host,
               int dht_pin, int dht_type, const char *project_id,
               const char *client_email, const char *private_key,
               const char *spreadsheet_id);
    Display display;
    Joystick joystick;
    WiFiManager wifi;
    MqttManager mqtt;
    Ntp ntp;
    Weather weather;
    DhtSensor dht;
    SpreadsheetWriter spreadsheetWriter;
    vector<bool> wasSuspended;
    void updateMqtt();
    Direction mqttDirection;
    String mqttMessage;
    vector<TaskHandle_t> tasks;

  private:
    const char *ssid;
    const char *password;
};

void onCallback(char *topic, byte *payload, unsigned int length);

#endif
