#include "HardwareSerial.h"
#include "ntp.hpp"
#include "pixelboard.hpp"
#include "snake.hpp"
#include "task_switcher.hpp"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>


#define LEDS1_PIN 25
#define LEDS2_PIN 26
#define JOYSTICK_BUTTON_PIN 32
#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 35


const char *mqtt_user = "snake";
const char *mqtt_password = "tre]:7T\"gm:TZ5a";
const int mqtt_port = 8883;
const char *ssid = "pixel";
const char *password = "password";

TaskHandle_t SnakeHandle = NULL;
TaskHandle_t NtpHandle = NULL;
TaskHandle_t taskSwitcherHandle = NULL;
PixelBoard *pixelboardPtr;
vector<TaskHandle_t> tasks;

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFiClientSecure secureClient;
    secureClient.setInsecure();
    PubSubClient client(secureClient);


    PixelBoard *pixelboard =
        new PixelBoard(LEDS1_PIN, LEDS2_PIN, JOYSTICK_BUTTON_PIN,
                       JOYSTICK_X_PIN, JOYSTICK_Y_PIN, ssid, password, vector<TaskHandle_t>(),mqtt_user,mqtt_password,mqtt_port,client);
    pixelboardPtr = pixelboard;

    xTaskCreate(Snake, "Snake", 10000, pixelboardPtr, 1, &SnakeHandle);
    xTaskCreate(Ntp, "Ntp", 10000, pixelboardPtr, 1, &NtpHandle);
    xTaskCreate(TaskSwitcher, "TaskSwitcher", 10000, pixelboardPtr, 1,
                &taskSwitcherHandle);
    
    tasks = {SnakeHandle, NtpHandle};
    pixelboardPtr->tasks = tasks;

    // pixelboard->wifi.begin();
}

void loop() {}
