#include "snake.hpp"
#include "pixelboard.hpp"
#include <list>

using namespace std;

const int GRID_SIZE_X = 32;
const int GRID_SIZE_Y = 16;
const int SNAKE_START_LENGTH = 3;
const int GAME_SPEED_DELAY = 200;
const CRGB SNAKE_COLOR = CRGB::Green;
const CRGB FOOD_COLOR = CRGB::Blue;
const CRGB BACKGROUND_COLOR = CRGB::Black;
const CRGB WALL_COLOR = CRGB::Red;

void generateFood(int &foodX, int &foodY,
                  const list<pair<int, int>> &snakeBody) {
    do {
        foodX = random(1, GRID_SIZE_X - 1);
        foodY = random(1, GRID_SIZE_Y - 1);
    } while (any_of(
        snakeBody.begin(), snakeBody.end(), [&](const pair<int, int> &segment) {
            return segment.first == foodX && segment.second == foodY;
        }));
}

void Snake(void *pvParameters) {
    vTaskDelay(50);

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    list<pair<int, int>> snakeBody;
    int snakeLength = SNAKE_START_LENGTH;
    int snakeHeadX = GRID_SIZE_X / 2;
    int snakeHeadY = GRID_SIZE_Y / 2;
    Direction direction = RIGHT;
    Direction previousDirection = RIGHT;
    int foodX, foodY;
    bool gameOver = false;
    unsigned long lastMoveTime = 0;
    int previousFoodX = -1;
    int previousFoodY = -1;
    int previousSnakeHeadX = -1;
    int previousSnakeHeadY = -1;

    for (int x = 0; x < GRID_SIZE_X; ++x) {
        pb->display.setLed(x, 0, WALL_COLOR);
        pb->display.setLed(x, GRID_SIZE_Y - 1, WALL_COLOR);
    }
    for (int y = 0; y < GRID_SIZE_Y; ++y) {
        pb->display.setLed(0, y, WALL_COLOR);
        pb->display.setLed(GRID_SIZE_X - 1, y, WALL_COLOR);
    }
    for (int i = 0; i < snakeLength; ++i) {
        snakeBody.push_back({snakeHeadX - i, snakeHeadY});
    }

    generateFood(foodX, foodY, snakeBody);
    previousFoodX = foodX;
    previousSnakeHeadX = snakeHeadX;
    previousSnakeHeadY = snakeHeadY;

    for (const auto &segment : snakeBody) {
        pb->display.setLed(segment.first, segment.second, SNAKE_COLOR);
    }

    while (!gameOver) {
        Serial.print("a");
        // pb->joystick.update();
        // direction = pb->joystick.getCurrentDirection();
        // if (direction == NONE) {
        //     direction = previousDirection;
        // }
        //
        // if (millis() - lastMoveTime >= GAME_SPEED_DELAY) {
        //     lastMoveTime = millis();
        //
        //     switch (direction) {
        //     case DOWN:
        //         previousDirection = DOWN;
        //         snakeHeadY--;
        //         break;
        //     case UP:
        //         previousDirection = UP;
        //         snakeHeadY++;
        //         break;
        //     case LEFT:
        //         previousDirection = LEFT;
        //         snakeHeadX--;
        //         break;
        //     case RIGHT:
        //         previousDirection = RIGHT;
        //         snakeHeadX++;
        //         break;
        //     }
        //
        //     if (snakeHeadX < 1 || snakeHeadX >= GRID_SIZE_X - 1 ||
        //         snakeHeadY < 1 || snakeHeadY >= GRID_SIZE_Y - 1) {
        //         gameOver = true;
        //         break;
        //     }
        //
        //     if (snakeHeadX < 0 || snakeHeadX >= GRID_SIZE_X || snakeHeadY < 0 ||
        //         snakeHeadY >= GRID_SIZE_Y) {
        //         gameOver = true;
        //         break;
        //     }
        //     for (const auto &segment : snakeBody) {
        //         if (segment.first == snakeHeadX &&
        //             segment.second == snakeHeadY) {
        //             gameOver = true;
        //             break;
        //         }
        //     }
        //     if (gameOver)
        //         break;
        //
        //     snakeBody.push_front({snakeHeadX, snakeHeadY});
        //
        //     if (snakeHeadX == foodX && snakeHeadY == foodY) {
        //         snakeLength++;
        //         generateFood(foodX, foodY, snakeBody);
        //         pb->display.setLed(previousFoodX, previousFoodY,
        //                            BACKGROUND_COLOR);
        //     } else {
        //         pair<int, int> tail = snakeBody.back();
        //         snakeBody.pop_back();
        //         pb->display.setLed(tail.first, tail.second, BACKGROUND_COLOR);
        //     }
        //     pb->display.setLed(snakeHeadX, snakeHeadY, SNAKE_COLOR);
        //     pb->display.setLed(foodX, foodY, FOOD_COLOR);
        //
        //     previousFoodX = foodX;
        //     previousFoodY = foodY;
        //     previousSnakeHeadX = snakeHeadX;
        //     previousSnakeHeadY = snakeHeadY;
        // }

        vTaskDelay(10);
    }

    pb->display.clear();

    vTaskDelay(pdMS_TO_TICKS(2000));
}
