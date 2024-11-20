#include <Arduino.h>
#include <gtest/gtest.h>
#include "button.hpp"

// Add the implementation file for EntprellterTaster
#include "button.cpp"

EntprellterTaster* taster;

void setup() {
  // Serial setup, if needed, can be handled here
  Serial.begin(115200);
  delay(3000); // Ensure serial communication is ready
}

void loop() {
  // Leave loop empty as the test framework handles execution
}

// The setup function for tests
class ButtonTest : public ::testing::Test {
protected:
  void SetUp() override {
    taster = new EntprellterTaster(32);  // Set up button on pin 32
  }

  void TearDown() override {
    delete taster;  // Clean up after each test
  }
};

// Test for short press
TEST_F(ButtonTest, ButtonPress) {
  Serial.println("Please press the button connected to pin 32.");
  delay(5000);  // Wait for 5 seconds for manual button press
  taster->aktualisieren();
  EXPECT_TRUE(taster->istGedrueckt());   // Check if the button is pressed
  EXPECT_TRUE(taster->wurdeGedrueckt()); // Check if the button was pressed
}

// Test for long press
TEST_F(ButtonTest, LongPress) {
  Serial.println("Please press and hold the button for at least 1 second.");
  delay(1500);  // Wait for 1.5 seconds for manual long press
  taster->aktualisieren();
  EXPECT_TRUE(taster->istLangGedrueckt(1000));  // Check if long press is detected
}