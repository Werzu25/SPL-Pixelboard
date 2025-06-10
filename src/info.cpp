#include "info.hpp"
#include "pixelboard.hpp"

void Info(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(500));

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);

    while (1) {
        while (1) {
            if (pb->wasSuspended[1] == true) {
                pb->wasSuspended[1] = false;
                break;
            }

            static int xOffset = 32;
            static unsigned long lastUpdate = 0;
            const unsigned long updateInterval = 100;
            const String message = "Hello World!";

            if (millis() - lastUpdate > updateInterval) {
                lastUpdate = millis();

                pb->display.print(message, 0, xOffset);

                xOffset--;
                if (xOffset < -(message.length() * 6)) {
                    xOffset = 32;
                }
            }

            vTaskDelay(pdMS_TO_TICKS(10));
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
