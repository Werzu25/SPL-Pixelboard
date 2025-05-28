#ifndef MENU_HPP
#define MENU_HPP
#include "display.hpp"
#include "pixelboard.hpp"

void Menu(void *pvParameters);
void drawMenuField(Display display, int x, int y);
void switchMenuField(PixelBoard *pb, vector<vector<int>> &menuFieldBorders);

#endif
