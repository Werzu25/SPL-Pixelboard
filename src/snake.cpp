#include "snake.hpp"
#include "pixelboard.hpp"
#include <list>
#include <FastLED.h>           // for CHSV()

using namespace std;

const int GRID_SIZE_X        = 32;
const int GRID_SIZE_Y        = 16;
const int SNAKE_START_LENGTH = 3;
const int GAME_SPEED_DELAY   = 200;
const CRGB FOOD_COLOR        = CRGB::Blue;
const CRGB BACKGROUND_COLOR  = CRGB::Black;
const CRGB WALL_COLOR        = CRGB::Red;
Direction payload_Direction = NONE;

// remove this fixed‐color:
// const CRGB SNAKE_COLOR = CRGB::Violet;

// rolling hue + independent hue‐timer:
// speed up the color loop (smaller interval)
static uint8_t  snakeHue            = 0;
static unsigned long lastHueUpdate = 0;
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

void onCallback(char *topic, byte *payload, unsigned int length) {
    Serial.printf("Message arrived [%s]: ", topic);
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);
    if (String(topic) == "snake/input_direction") {
        if (message == "UP") {
            payload_Direction = UP;
        } else if (message == "DOWN") {
            payload_Direction = DOWN;
        } else if (message == "LEFT") {
            payload_Direction = LEFT;
        } else if (message == "RIGHT") {
            payload_Direction = RIGHT;
        } else {
            payload_Direction = NONE;
        }
    }
}

void Snake(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(100));

    PixelBoard *pb = static_cast<PixelBoard *>(pvParameters);
    bool wasSuspended = false;
    
    while (true) {
        pb->mqtt.connect(onCallback);
        pb->mqtt.subscribe("snake/input_direction");

        list<pair<int, int>> snakeBody;
        int snakeLength = SNAKE_START_LENGTH;
        int snakeHeadX = GRID_SIZE_X / 2;
        int snakeHeadY = GRID_SIZE_Y / 2;
        Direction direction = RIGHT;
        Direction previousDirection = RIGHT;
        Direction joystickDirection = NONE;
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
            pb->display.setLed(segment.first, segment.second, CHSV(snakeHue, 255, 255));
        }
        pb->display.setLed(foodX, foodY, FOOD_COLOR);

        while (!gameOver) {
            joystickDirection = NONE;
            payload_Direction = NONE;
            unsigned long now = millis();

            if (now - lastHueUpdate >= HUE_INTERVAL) {
                lastHueUpdate = now;
                snakeHue++;
            }

            pb->mqtt.client.loop();
            vector<bool> wasSuspended = pb->getWasSuspended();
            if (wasSuspended[1] == true) {
                wasSuspended[1] = false;
                pb->setWasSuspended(wasSuspended);
                break;
            }

            pb->joystick.update();
            joystickDirection = pb->joystick.getCurrentDirection();
            
            if (joystickDirection != NONE) {
                direction = joystickDirection;
            }

            if (payload_Direction != NONE) {
                direction = payload_Direction;
            }

            Serial.print("Joystick Direction: ");
            Serial.println(joystickDirection);
            Serial.print("Payload Direction: ");
            Serial.println(payload_Direction);
        
            if (direction == NONE) {
                direction = previousDirection;
            }

            Serial1.println(direction);
            
            // only move the snake every GAME_SPEED_DELAY:
            if (now - lastMoveTime >= GAME_SPEED_DELAY) {
                lastMoveTime = now;
                
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
                    if (segment.first == nextHeadX && segment.second == nextHeadY) {
                        collision = true;
                        break;
                    }
                }
                
                if (collision) {
                    gameOver = true;
                    break;
                }

                // draw new head with current hue
                CRGB dynColor = CHSV(snakeHue, 255, 255);
                snakeHeadX = nextHeadX;
                snakeHeadY = nextHeadY;
                snakeBody.push_front({snakeHeadX, snakeHeadY});
                pb->display.setLed(snakeHeadX, snakeHeadY, dynColor);

                if (snakeHeadX == foodX && snakeHeadY == foodY) {
                    // ate food: grow, pick a new food spot
                    snakeLength++;
                    int oldFoodX = foodX, oldFoodY = foodY;
                    generateFood(foodX, foodY, snakeBody);

                    // don't clear oldFoodX/oldFoodY – it's now snake
                    // pb->display.setLed(oldFoodX, oldFoodY, BACKGROUND_COLOR);

                    pb->display.setLed(foodX, foodY, FOOD_COLOR);
                } else {
                    // normal tail removal
                    auto tail = snakeBody.back();
                    snakeBody.pop_back();
                    pb->display.setLed(tail.first, tail.second, BACKGROUND_COLOR);
                    // re‐draw existing food
                    pb->display.setLed(foodX, foodY, FOOD_COLOR);
                }

                previousFoodX = foodX;
                previousFoodY = foodY;
                previousSnakeHeadX = snakeHeadX;
                previousSnakeHeadY = snakeHeadY;
            }

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
                    pb->display.setLed(segment.first, segment.second, WALL_COLOR);
                }
                vTaskDelay(pdMS_TO_TICKS(200));
                for (const auto &segment : deathAnimation) {
                    pb->display.setLed(segment.first, segment.second, BACKGROUND_COLOR);
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

void SnakeMultiplayer(void *pvParameters) {

}
