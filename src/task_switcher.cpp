#include "task_switcher.hpp"
#include "HardwareSerial.h"
#include "pixelboard.hpp"
#include <Arduino.h>
#include <vector>

using namespace std;

void TaskSwitcher(void *pvParameters) {
    vTaskDelay(50);

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    vector<TaskHandle_t> tasks = pb->getTasks();
    int activeTask = 0;
    static unsigned long lastCheckTime = 0;

    while (1) {
        Serial.print("c");
        pb->joystick.update();

        if (millis() - lastCheckTime >= 50) {
            if (pb->joystick.wasPressed()) {
                activeTask = (activeTask + 1) % tasks.size();
                Serial.print("activeTask: ");
                Serial.println(activeTask);
            }

            for (size_t i = 0; i < tasks.size(); i++) {
                if (i == activeTask) {
                    vTaskResume(tasks[i]);
                } else {
                    vTaskSuspend(tasks[i]);
                }
            }

            lastCheckTime = millis();
        }

        vTaskDelay(10);
    }
}
