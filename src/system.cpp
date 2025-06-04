#include "system.hpp"
#include "HardwareSerial.h"
#include "esp32-hal.h"
#include "pixelboard.hpp"
#include <Arduino.h>
#include <vector>

using namespace std;

void System(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(100));

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    vector<TaskHandle_t> tasks = pb->tasks;

    int activeTask = 0;
    int lastActiveTask = activeTask;
    static unsigned long lastCheckTime = 0;

    vTaskDelay(pdMS_TO_TICKS(100));

    for (size_t i = 0; i < tasks.size(); i++) {
        if (i == activeTask) {
            vTaskResume(tasks[i]);
        } else {
            vTaskSuspend(tasks[i]);
        }
    }

    while (1) {
        pb->joystick.update();

        if (millis() - lastCheckTime >= 50) {
            if (pb->joystick.wasPressed()) {
                vTaskSuspend(tasks[activeTask]);
                pb->wasSuspended[activeTask] = true;
                activeTask = (activeTask + 1) % tasks.size();
                delay(100);
                pb->display.clear();
                Serial.print("clear");
                vTaskResume(tasks[activeTask]);
            }

            lastCheckTime = millis();
        }

        if (lastActiveTask != activeTask) {
            Serial.print("[System] Switching from Task ");
            Serial.print(lastActiveTask);
            Serial.print(" to Task ");
            Serial.println(activeTask);
            lastActiveTask = activeTask;
        }

        pb->mqtt.client.loop();

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
