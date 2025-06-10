#include "system.hpp"
#include "HardwareSerial.h"
#include "pixelboard.hpp"
#include <Arduino.h>
#include <vector>

using namespace std;

struct item {
    int id;
    int active;
};

void System(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(10));

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);

    vector<TaskHandle_t> tasks = pb->tasks;
    for (size_t i = 0; i < tasks.size(); i++) {
        vTaskSuspend(tasks[i]);
    }

    int x = 0;
    int y = 0;
    int lastX = -1;
    int lastY = -1;
    bool menu = true;
    int activeTask = 0;
    Direction direction = NONE;

    while (1) {
        pb->display.show();
        pb->mqtt.client.loop();
        pb->updateMqtt();
        pb->joystick.update();

        if (menu) {
            if (x != lastX || y != lastY) {
                pb->display.rectangle(16, 8, CRGB::White, false, 0, 0);
                pb->display.setLed(4, 4, CRGB::Green);
                pb->display.setLed(5, 4, CRGB::Green);
                pb->display.setLed(6, 4, CRGB::Green);
                pb->display.setLed(7, 4, CRGB::Green);
                pb->display.setLed(8, 4, CRGB::Green);
                pb->display.setLed(4, 3, CRGB::Green);
                pb->display.setLed(5, 3, CRGB::Green);
                pb->display.setLed(6, 3, CRGB::Green);
                pb->display.setLed(7, 3, CRGB::Green);
                pb->display.setLed(8, 3, CRGB::Green);
                pb->display.setLed(10, 4, CRGB::Blue);
                pb->display.setLed(10, 3, CRGB::Blue);
                pb->display.setLed(11, 4, CRGB::Blue);
                pb->display.setLed(11, 3, CRGB::Blue);
                pb->display.rectangle(16, 8, CRGB::White, false, 16, 0);
                pb->display.setLed(23, 2, CRGB::Yellow);
                pb->display.setLed(23, 3, CRGB::Yellow);
                pb->display.setLed(23, 4, CRGB::Yellow);
                pb->display.setLed(23, 5, CRGB::Yellow);
                pb->display.setLed(24, 2, CRGB::Yellow);
                pb->display.setLed(24, 3, CRGB::Yellow);
                pb->display.setLed(24, 4, CRGB::Yellow);
                pb->display.setLed(24, 5, CRGB::Yellow);
                pb->display.rectangle(16, 8, CRGB::White, false, 0, 8);
                pb->display.rectangle(16, 8, CRGB::White, false, 16, 8);
                pb->display.rectangle(16, 8, CRGB::Red, false, x * 16, y * 8);
            }

            lastY = y;
            lastX = x;

            if (pb->mqttDirection != NONE) {
                direction = pb->mqttDirection;
            }
            if (pb->joystick.getCurrentDirection() != NONE) {
                direction = pb->joystick.getCurrentDirection();
            }

            switch (direction) {
            case DOWN:
                if (y > 0) {
                    y--;
                }
                break;
            case UP:
                if (y < 1) {
                    y++;
                }
                break;
            case LEFT:
                if (x > 0) {
                    x--;
                }
                break;
            case RIGHT:
                if (x < 1) {
                    x++;
                }
                break;
            case NONE:
                break;
            }

            direction = NONE;

            if (pb->joystick.wasPressed() || pb->mqttMessage == "ENTER") {
                activeTask = x + y * 2;
                Serial.print("[System] switch to task ");
                Serial.println(activeTask);
                menu = false;
                pb->display.clear();
                vTaskResume(tasks[activeTask]);
            }
        } else {
            if (pb->joystick.wasPressed() || pb->mqttMessage == "ENTER") {
                vTaskSuspend(tasks[activeTask]);
                pb->wasSuspended[activeTask] = true;
                pb->display.clear();
                menu = true;
                lastX = -1;
                lastY = -1;
                Serial.println("[System] show menu");
            }
        }

        static unsigned long lastUpdate = 0;
        const unsigned long updateInterval = 60000;

        if (millis() - lastUpdate > updateInterval) {
            lastUpdate = millis();

            String timeString =
                String("Time: ") + String(pb->ntp.getCurrentTime());
            String temperatureString = String("Temperature: ") +
                                       String(pb->dht.getTemperature()) +
                                       String(" °C");
            String humidityString = String("Humidity: ") +
                                    String(pb->dht.getHumidity()) +
                                    String(" %");

            static char *cells[4] = {
                (char *)timeString.c_str(),
                (char *)temperatureString.c_str(),
                (char *)humidityString.c_str(),
            };
            pb->spreadsheetWriter.sendData(cells);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
