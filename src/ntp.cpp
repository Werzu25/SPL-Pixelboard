#include "ntp.hpp"
#include "HardwareSerial.h"
#include <Arduino.h>

void Ntp(void *pvParameters) {
    vTaskDelay(50);

    while (1) {
        Serial.print("b");
        vTaskDelay(10);
    };
}
