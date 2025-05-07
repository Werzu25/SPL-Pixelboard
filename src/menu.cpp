#include "menu.hpp"
#include "HardwareSerial.h"
#include <Arduino.h>
#include "pixelboard.hpp"
#include <time.h>

void Menu(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(100));
    
    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
