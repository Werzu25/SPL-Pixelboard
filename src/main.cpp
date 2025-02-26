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
#include "button.hpp"
#include "joystick.h"

EntprellterTaster taster(32);  // Initialisiert den Taster an Pin 32
Joystick joystick(25, 34, 35); // Initialisiert den Joystick an den Pins 25, 34 und 35

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
    joystick.aktualisieren();

    if (taster.wurdeGedrueckt()) {
        Serial.println("Kurzer Tastendruck erkannt");
    }

    if (taster.wurdeLangGedrueckt()) {
        Serial.println("Langer Tastendruck erkannt");
    }

    int x = joystick.getX();
    int y = joystick.getY();
    bool joystickButtonPressed = joystick.istGedrueckt();

    Serial.print("Joystick X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.println(y);
    Serial.print("Joystick Button ist ");
    Serial.println(joystickButtonPressed ? "GEDRUECKT" : "NICHT GEDRUECKT");

    delay(500);  // Verzögerung von 500ms
}