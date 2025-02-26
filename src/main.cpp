#include <Arduino.h>

// Pin definition for joystick button
#define JOYSTICK_BTN_PIN 2

// Task handles
TaskHandle_t taskAHandle = NULL;
TaskHandle_t taskBHandle = NULL;
TaskHandle_t taskCHandle = NULL;

// Current active task (0 = A, 1 = B, 2 = C)
volatile int activeTask = 0;

// Semaphore to protect the serial output
SemaphoreHandle_t serialSemaphore;

// Task A: Print 'A' every 1 second
void TaskA(void *pvParameters) {
    delay(10);
    while (1) {
        if (activeTask == 0) {
            if (xSemaphoreTake(serialSemaphore, portMAX_DELAY) == pdTRUE) {
                Serial.println("A");
                xSemaphoreGive(serialSemaphore);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
    }
}

// Task B: Print 'B' every 2 seconds
void TaskB(void *pvParameters) {
    delay(10);
    while (1) {
        if (activeTask == 1) {
            if (xSemaphoreTake(serialSemaphore, portMAX_DELAY) == pdTRUE) {
                Serial.println("B");
                xSemaphoreGive(serialSemaphore);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds delay
    }
}

// Task C: Print 'C' every 5 seconds
void TaskC(void *pvParameters) {
    delay(10);
    while (1) {
        if (activeTask == 2) {
            if (xSemaphoreTake(serialSemaphore, portMAX_DELAY) == pdTRUE) {
                Serial.println("C");
                xSemaphoreGive(serialSemaphore);
            }
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
        activeTask = (activeTask + 1) % 3;  // Cycle through tasks (0, 1, 2)
        
        if (xSemaphoreTakeFromISR(serialSemaphore, NULL) == pdTRUE) {
            Serial.print("Switching to task ");
            Serial.println(activeTask == 0 ? "A" : activeTask == 1 ? "B" : "C");
            xSemaphoreGiveFromISR(serialSemaphore, NULL);
        }
    }
    last_interrupt_time = interrupt_time;
}

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect
    }
    
    // Create semaphore for serial output
    serialSemaphore = xSemaphoreCreateMutex();
    
    // Configure joystick button pin as input with internal pull-up
    pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(JOYSTICK_BTN_PIN), buttonPressed, FALLING);
    
    // Create tasks
    xTaskCreate(
        TaskA,
        "TaskA",
        128,
        NULL,
        1,
        &taskAHandle
    );
    
    xTaskCreate(
        TaskB,
        "TaskB",
        128,
        NULL,
        1,
        &taskBHandle
    );
    
    xTaskCreate(
        TaskC,
        "TaskC",
        128,
        NULL,
        1,
        &taskCHandle
    );
    
    Serial.println("System initialized. Task A active.");
    
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
}

void loop() {
    // Empty. Things are done in Tasks.
}