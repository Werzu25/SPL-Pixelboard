#include "task_switcher.hpp"
#include "HardwareSerial.h"
#include "pixelboard.hpp"
#include <Arduino.h>
#include <vector>

using namespace std;

void TaskSwitcher(void *pvParameters) {
    // Give other tasks time to initialize
    vTaskDelay(pdMS_TO_TICKS(100));

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    vector<TaskHandle_t> tasks = pb->getTasks();

    Serial.print("Number of tasks: ");
    Serial.println(tasks.size());

    int activeTask = 0;
    static unsigned long lastCheckTime = 0;

    // Wait for all tasks to be created
    vTaskDelay(pdMS_TO_TICKS(100));

    // Initially suspend all tasks except the first one
    for (size_t i = 0; i < tasks.size(); i++) {
        if (tasks[i] == NULL) {
            Serial.print("Task ");
            Serial.print(i);
            Serial.println(" is NULL!");
            continue;
        }
        
        if (i == activeTask) {
            Serial.print("Resuming task ");
            Serial.println(i);
            vTaskResume(tasks[i]);
        } else {
            Serial.print("Suspending task ");
            Serial.println(i);
            vTaskSuspend(tasks[i]);
        }
    }

    while (1) {
        pb->joystick.update();

        if (millis() - lastCheckTime >= 50) {
            if (pb->joystick.wasPressed()) {
                // Suspend current task
                if (tasks[activeTask] != NULL && tasks[activeTask] != xTaskGetCurrentTaskHandle()) {
                    Serial.print("Suspending task ");
                    Serial.println(activeTask);
                    vTaskSuspend(tasks[activeTask]);
                }

                // Switch to next task
                activeTask = (activeTask + 1) % tasks.size();
                Serial.print("Switching to task ");
                Serial.println(activeTask);

                // Resume new task
                if (tasks[activeTask] != NULL && tasks[activeTask] != xTaskGetCurrentTaskHandle()) {
                    Serial.print("Resuming task ");
                    Serial.println(activeTask);
                    vTaskResume(tasks[activeTask]);
                }
            }

            lastCheckTime = millis();
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
