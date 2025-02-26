#include "display.hpp"
#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 26
#define NUM_LEDS 512
#define PANEL_WIDTH 32
#define PANEL_HEIGHT 8

CRGB leds[NUM_LEDS];

TaskHandle_t TaskAHandle = NULL;
TaskHandle_t TaskBHandle = NULL;
TaskHandle_t TaskCHandle = NULL;

void TaskA(void *pvParameters) {
    while (true) {
        Serial.println("a");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void TaskB(void *pvParameters) {
    while (true) {
        Serial.println("b");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void TaskC(void *pvParameters) {
    while (true) {
        Serial.println("c");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("pixelboard initalized");

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(50);
    FastLED.clear(true);
    FastLED.show();

    // xTaskCreate(TaskA, "TaskA", 1000, NULL, 1, &TaskAHandle);
    // xTaskCreate(TaskB, "TaskB", 1000, NULL, 1, &TaskBHandle);
    // xTaskCreate(TaskC, "TaskC", 1000, NULL, 1, &TaskCHandle);
}

void loop() {
    // for (int i = 0; i < 32; i++) {
    //     setLed(i, 0, CRGB::Red, leds);
    //     FastLED.clear(true);
    // }
    setLed(0, 0, CRGB::Blue, leds);
    setLed(1, 0, CRGB::Blue, leds);
    setLed(2, 0, CRGB::Blue, leds);
    setLed(1, 1, CRGB::Yellow, leds);
    setLed(7, 7, CRGB::Yellow, leds);
    setLed(7, 31, CRGB::Yellow, leds);
    leds[0] = CRGB::Green;
    FastLED.show();

    //still doesnt work kp wieso
}
