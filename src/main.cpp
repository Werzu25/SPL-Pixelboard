#include <Arduino.h>
#include "button.hpp"  // Make sure to use correct case for the filename

// Global variables
TaskHandle_t taskA_Handle = NULL;
TaskHandle_t taskB_Handle = NULL;
TaskHandle_t taskC_Handle = NULL;
volatile char currentTask = 'A';  // Start with task A
EntprellterTaster taster;  // Declare the Button object

// Task functions
void taskA(void *parameter) {
    delay(10);
    while (true) {
        if (currentTask == 'A') {
            Serial.println("Task A running");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void taskB(void *parameter) {
    delay(10);
    while (true) {
        if (currentTask == 'B') {
            Serial.println("Task B running");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void taskC(void *parameter) {
    delay(10);
    while (true) {
        if (currentTask == 'C') {
            Serial.println("Task C running");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    
    // Create all three tasks
    xTaskCreate(
        taskA,
        "TaskA",
        2048,
        NULL,
        1,
        &taskA_Handle
    );
    
    xTaskCreate(
        taskB,
        "TaskB",
        2048,
        NULL,
        1,
        &taskB_Handle
    );
    
    xTaskCreate(
        taskC,
        "TaskC",
        2048,
        NULL,
        1,
        &taskC_Handle
    );
    
    // Initialize button
    taster.initialisieren();
}

void loop() {
    taster.aktualisieren();
}