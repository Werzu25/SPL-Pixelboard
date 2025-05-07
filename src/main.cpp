#include "HardwareSerial.h"
#include "menu.hpp"
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

TaskHandle_t MenuHandle = NULL;
TaskHandle_t SnakeHandle = NULL;
TaskHandle_t taskSwitcherHandle = NULL;
PixelBoard *pixelboardPtr;
vector<TaskHandle_t> tasks;

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {
        ;
    }
    
    while (Serial.available()) {
        Serial.read();
    }
    WiFiClientSecure secureClient;
    secureClient.setInsecure();
    PubSubClient client(secureClient);

    PixelBoard *pixelboard =
        new PixelBoard(LEDS1_PIN, LEDS2_PIN, JOYSTICK_BUTTON_PIN,
                       JOYSTICK_X_PIN, JOYSTICK_Y_PIN, ssid, password, vector<TaskHandle_t>(), {false, false},mqtt_user,mqtt_password,mqtt_port,client);
    pixelboardPtr = pixelboard;

    Serial.println("[Setup] Connecting to WiFi...");
    pixelboard->wifi.begin();

    Serial.println("[Setup] Creating tasks...");
    xTaskCreate(Menu, "Menu", 10000, pixelboardPtr, 1, &MenuHandle);
    vTaskSuspend(MenuHandle);
    delay(10);

    xTaskCreate(Snake, "Snake", 10000, pixelboardPtr, 1, &SnakeHandle);
    vTaskSuspend(SnakeHandle);
    delay(10);

    xTaskCreate(TaskSwitcher, "TaskSwitcher", 10000, pixelboardPtr, 1,
                &taskSwitcherHandle);
    
    tasks = {MenuHandle, SnakeHandle};
    pixelboardPtr->tasks = tasks;

    Serial.println("[Setup] Initialization complete");
}

void loop() {}
