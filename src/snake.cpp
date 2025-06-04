#include "snake.hpp"
#include "joystick.hpp"
#include "pixelboard.hpp"
#include <ctime>
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

static const unsigned long HUE_INTERVAL = 5;

void generateFood(int &foodX, int &foodY,
                  const list<pair<int, int>> &snakeBody) {
    do {
        foodX = random(0, GRID_SIZE_X);
        foodY = random(0, GRID_SIZE_Y);
    } while (any_of(
        snakeBody.begin(), snakeBody.end(), [&](const pair<int, int> &segment) {
            return segment.first == foodX && segment.second == foodY;
        }));
}

void Snake(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(500));

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    bool wasSuspended = false;

    static uint8_t snakeHue = 0;
    static unsigned long lastHueUpdate = 0;

    while (true) {
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

        for (int i = 0; i < snakeLength; ++i) {
            snakeBody.push_back({snakeHeadX - i, snakeHeadY});
        }
        generateFood(foodX, foodY, snakeBody);

        for (const auto &segment : snakeBody) {
            pb->display.setLed(segment.first, segment.second, SNAKE_COLOR);
        }
        pb->display.setLed(foodX, foodY, FOOD_COLOR);

        pb->display.show();

        while (!gameOver) {
            if (pb->wasSuspended[1] == true) {
                pb->wasSuspended[1] = false;
                break;
            }

            unsigned long now = millis();
            if (now - lastHueUpdate >= HUE_INTERVAL) {
                lastHueUpdate = now;
                snakeHue++;
            }

            pb->updateMqttDiretion();
            if (pb->mqttDirection != NONE) {
                direction = pb->mqttDirection;
            } else {
                pb->joystick.update();
                direction = pb->joystick.getCurrentDirection();
            }

            if (direction == NONE) {
                direction = previousDirection;
            }

            if (millis() - lastMoveTime >= GAME_SPEED_DELAY) {
                lastMoveTime = millis();

                int nextHeadX = snakeHeadX;
                int nextHeadY = snakeHeadY;

                Direction newDirection = direction;
                if (direction == NONE) {
                    newDirection = previousDirection;
                } else {
                    if ((direction == LEFT && previousDirection == RIGHT) ||
                        (direction == RIGHT && previousDirection == LEFT) ||
                        (direction == UP && previousDirection == DOWN) ||
                        (direction == DOWN && previousDirection == UP)) {
                        newDirection = previousDirection;
                    }
                }

                switch (newDirection) {
                case DOWN:
                    previousDirection = DOWN;
                    nextHeadY--;
                    break;
                case UP:
                    previousDirection = UP;
                    nextHeadY++;
                    break;
                case LEFT:
                    previousDirection = LEFT;
                    nextHeadX--;
                    break;
                case RIGHT:
                    previousDirection = RIGHT;
                    nextHeadX++;
                    break;
                }

                if (nextHeadX < 0) {
                    nextHeadX = GRID_SIZE_X - 1;
                } else if (nextHeadX >= GRID_SIZE_X) {
                    nextHeadX = 0;
                }
                if (nextHeadY < 0) {
                    nextHeadY = GRID_SIZE_Y - 1;
                } else if (nextHeadY >= GRID_SIZE_Y) {
                    nextHeadY = 0;
                }

                // Check collision with snake body
                bool collision = false;
                for (const auto &segment : snakeBody) {
                    if (segment.first == nextHeadX &&
                        segment.second == nextHeadY) {
                        collision = true;
                        break;
                    }
                }

                if (collision) {
                    gameOver = true;
                    break;
                }

                CRGB dynColor = CHSV(snakeHue, 255, 255);
                snakeHeadX = nextHeadX;
                snakeHeadY = nextHeadY;

                snakeBody.push_front({snakeHeadX, snakeHeadY});

                if (snakeHeadX == foodX && snakeHeadY == foodY) {
                    snakeLength++;
                    generateFood(foodX, foodY, snakeBody);
                    pb->display.setLed(previousFoodX, previousFoodY,
                                       BACKGROUND_COLOR);
                } else {
                    pair<int, int> tail = snakeBody.back();
                    snakeBody.pop_back();
                    pb->display.setLed(tail.first, tail.second,
                                       BACKGROUND_COLOR);
                }
                pb->display.setLed(snakeHeadX, snakeHeadY, dynColor);

                pb->display.setLed(foodX, foodY, FOOD_COLOR);

                previousFoodX = foodX;
                previousFoodY = foodY;
                previousSnakeHeadX = snakeHeadX;
                previousSnakeHeadY = snakeHeadY;
            }

            pb->display.show();

            vTaskDelay(pdMS_TO_TICKS(10));
        }

        vector<pair<int, int>> deathAnimation;
        if (gameOver) {
            pb->display.clear();

            for (const auto &segment : snakeBody) {
                deathAnimation.push_back(segment);
            }

            for (int i = 0; i < 3; i++) {
                for (const auto &segment : deathAnimation) {
                    pb->display.setLed(segment.first, segment.second,
                                       WALL_COLOR);
                }
                vTaskDelay(pdMS_TO_TICKS(200));
                for (const auto &segment : deathAnimation) {
                    pb->display.setLed(segment.first, segment.second,
                                       BACKGROUND_COLOR);
                }
                vTaskDelay(pdMS_TO_TICKS(200));
            }
        }

        pb->display.clear();

        snakeBody.clear();
        deathAnimation.clear();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
