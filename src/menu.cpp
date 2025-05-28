#include "menu.hpp"
#include "HardwareSerial.h"
#include <Arduino.h>
#include "pixelboard.hpp"
#include <time.h>


const int menuFieldCount = 4;
const int menuFieldWidth = 15;
const int menuFieldHeight = 7;
// currentField = {col, row}
int currentField[2] = {0, 0};

// add grid dimensions
const int menuCols = 2;
const int menuRows = 2;

void Menu(void *pvParameters)
{

    vTaskDelay(pdMS_TO_TICKS(100));

    while (1)
    {
        PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
        vector<vector<int>> menuFieldBorders;

        int menuFieldX = 0;
        int menuFieldY = 0;
        for (int i = 0; i < menuFieldCount; i++)
        {
            drawMenuField(pb->display, menuFieldX, menuFieldY);
            menuFieldBorders.push_back({menuFieldX, menuFieldY});
            menuFieldX += menuFieldWidth + 2;
            if (menuFieldX >= 32)
            {
                menuFieldX = 0;
                menuFieldY += menuFieldHeight + 2;
            }
        }

        while (1)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            switchMenuField(pb, menuFieldBorders);
        }
    }
}

void drawMenuField(Display display, int x, int y)
{
    display.drawRect(x, y, menuFieldWidth, menuFieldHeight, CRGB::White);
    display.drawRect(x + 1, y + 1, menuFieldWidth - 2, menuFieldHeight - 2, CRGB::Black);
}

void switchMenuField(PixelBoard *pb, vector<vector<int>> &menuFieldBorders)
{
    Direction joystickDirection = pb->joystick.getCurrentDirection();

    // erase previous highlight…
    int prevIdx = currentField[1] * menuCols + currentField[0];
    pb->display.drawRect(menuFieldBorders[prevIdx][0],
                         menuFieldBorders[prevIdx][1],
                         menuFieldWidth, menuFieldHeight,
                         CRGB::White);
    pb->display.drawRect(menuFieldBorders[prevIdx][0] + 1,
                         menuFieldBorders[prevIdx][1] + 1,
                         menuFieldWidth - 2, menuFieldHeight - 2,
                         CRGB::Black);

    switch (joystickDirection)
    {
    case LEFT:
        if (currentField[0] > 0) currentField[0]--;
        break;
    case RIGHT:
        if (currentField[0] < menuCols - 1) currentField[0]++;
        break;
    case UP:    // invert: pushing UP now moves you DOWN
        if (currentField[1] < menuRows - 1) currentField[1]++;
        break;
    case DOWN:  // invert: pushing DOWN now moves you UP
        if (currentField[1] > 0) currentField[1]--;
        break;
    default:
        return;
    }

    // draw new highlight…
    int newIdx = currentField[1] * menuCols + currentField[0];
    pb->display.drawRect(menuFieldBorders[newIdx][0],
                         menuFieldBorders[newIdx][1],
                         menuFieldWidth, menuFieldHeight,
                         CRGB::AliceBlue);
    pb->display.drawRect(menuFieldBorders[newIdx][0] + 1,
                         menuFieldBorders[newIdx][1] + 1,
                         menuFieldWidth - 2, menuFieldHeight - 2,
                         CRGB::Black);
}