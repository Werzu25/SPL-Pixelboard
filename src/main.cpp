#include <display.hpp>
#include <Arduino.h>
#include <FastLED.h>
#include <joystick.hpp>
#include <weatherService.hpp>
#include <timeService.hpp>

#include <FastLED.h>

#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>

/*
// For LED Matrix
#define LED_PIN        25
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   68
#define MATRIX_HEIGHT  7
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

cLEDText ScrollingMsg;


#define LED_PIN 26
#define NUM_LEDS 512
#define PANEL_WIDTH 32
#define PANEL_HEIGHT 8
*/
#define JOYSTICK_BTN_PIN 32
#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 35

TaskHandle_t taskAHandle = NULL;
TaskHandle_t taskBHandle = NULL;
TaskHandle_t taskCHandle = NULL;

// Current active task (0 = A, 1 = B, 2 = C)
volatile int activeTask = 0;

// Create a Joystick object
Joystick joystick(JOYSTICK_BTN_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
TimeService timeService;

// Function to check joystick button
void checkJoystickButton() {
  static unsigned long lastCheckTime = 0;
  
  // Check the button only every 50ms to debounce
  if (millis() - lastCheckTime >= 50) {
    joystick.aktualisiere();
    
    // Check for long press
    if (joystick.wurdeLangGedrueckt()) {
      activeTask = (activeTask + 1) % 3; // Cycle through tasks
      Serial.print("Switching to task ");
      Serial.println(activeTask == 0 ? "A" : activeTask == 1 ? "B" : "C");
    }
    
    lastCheckTime = millis();
  }
}

// Task A: Print 'A' every 1 second
void TaskA(void *pvParameters) {
  while (1) {
    checkJoystickButton();
    
    if (activeTask == 0) {
      WeatherData weatherData = getWeatherData();  // Changed from getWeatherData() to getWeather()
      Serial.println(weatherData.temp_c);
    }
    
    vTaskDelay(pdMS_TO_TICKS(10000)); // 10 second delay
  }
}

void TaskB(void *pvParameters) {
  while (1) {
    checkJoystickButton();
    
    if (activeTask == 1) {
      timeService.printLocalTime();
    }
    
    vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds delay
  }
}

void TaskC(void *pvParameters) {
  while (1) {
    checkJoystickButton();
    
    if (activeTask == 2) {
      Serial.println("C");
    }
    
    vTaskDelay(pdMS_TO_TICKS(5000)); // 5 seconds delay
  }
}

void setup1() {
  // Initialize serial communication
  Serial.begin(115200);
  setupWeather();
  timeService.begin();
  timeService.updateTime();
  
  // Initialize joystick pins
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  
  Serial.println("System initializing...");
  
  // Create tasks with sufficient stack size
  xTaskCreate(
    TaskA,          // Task function
    "TaskA",        // Task name
    8192,           // Stack size (in words)
    NULL,           // Task parameters
    1,              // Priority
    &taskAHandle    // Task handle
  );
  
  xTaskCreate(
    TaskB,
    "TaskB",
    2048,
    NULL,
    1,
    &taskBHandle
  );
  
  xTaskCreate(
    TaskC,
    "TaskC",
    2048,
    NULL,
    1,
    &taskCHandle
  );
  
  Serial.println("System initialized. Task A active.");
}

void loop1() {
  // Empty. All the work is done in Tasks.
}