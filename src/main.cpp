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

// Current active task (0 = A, 1 = B, 2 = C)
volatile int activeTask = 0;

// Task A: Print 'A' every 1 second
void TaskA(void *pvParameters) {
    delay(10);
    while (1) {
        if (activeTask == 0) {
            Serial.println("A");
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
    }
}

void TaskB(void *pvParameters) {
    delay(10);
    while (1) {
        if (activeTask == 1) {
            Serial.println("B");
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds delay
    }
}

void TaskC(void *pvParameters) {
    delay(10);
    while (1) {
        if (activeTask == 2) {
            Serial.println("C");
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 seconds delay
    }
}

// ISR for joystick button
void buttonPressed() {
    // Debounce by ignoring interrupts for 200ms
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    if (interrupt_time - last_interrupt_time > 200) {
        activeTask = (activeTask + 1) % 3; // Cycle through tasks (0, 1, 2)
        
        Serial.print("Switching to task ");
        Serial.println(activeTask == 0 ? "A" : activeTask == 1 ? "B" : "C");
    }
    last_interrupt_time = interrupt_time;
}

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect
    }

    // Configure joystick button pin as input with internal pull-up
    pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(JOYSTICK_BTN_PIN), buttonPressed,
                    FALLING);

    // Create tasks
    xTaskCreate(TaskA, "TaskA", 128, NULL, 1, &taskAHandle);
    xTaskCreate(TaskB, "TaskB", 128, NULL, 1, &taskBHandle);
    xTaskCreate(TaskC, "TaskC", 128, NULL, 1, &taskCHandle);

    Serial.println("System initialized. Task A active.");

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
}

void loop() {
    // Empty. Things are done in Tasks.
}
