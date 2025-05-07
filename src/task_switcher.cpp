#include "task_switcher.hpp"
#include "HardwareSerial.h"
#include "pixelboard.hpp"
#include <Arduino.h>
#include <vector>

using namespace std;

void TaskSwitcher(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(100));

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    vector<TaskHandle_t> tasks = pb->getTasks();

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
                vector<bool> wasSuspended = pb->getWasSuspended();
                wasSuspended[activeTask] = true;
                pb->setWasSuspended(wasSuspended);
                activeTask = (activeTask + 1) % tasks.size();
                pb->display.clear();
                vTaskResume(tasks[activeTask]);
            }

            lastCheckTime = millis();
        }

        if (lastActiveTask != activeTask) {
            Serial.print("[TaskSwitcher] Switching from Task ");
            Serial.print(lastActiveTask);
            Serial.print(" to Task ");
            Serial.println(activeTask);
            lastActiveTask = activeTask;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
