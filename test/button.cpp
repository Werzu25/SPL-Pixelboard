#include "button.hpp"
#include <Arduino.h>
#include <unity.h>

// Mocking the digitalRead function
int mockPinState = HIGH;

// Function to simulate digitalRead
int mockDigitalRead(int pin) { return mockPinState; }

// Function to set the mock pin state
void setMockPinState(int state) { mockPinState = state; }

void test_initial_state() {
    EntprellterTaster button(2); // Assuming pin 2 for the button
    TEST_ASSERT_TRUE(button.istGedrueckt() == false);
    TEST_ASSERT_TRUE(button.wurdeGedrueckt() == false);
    TEST_ASSERT_TRUE(button.wurdeLangGedrueckt() == false);
}

void test_button_press() {
    EntprellterTaster button(2);

    // Simulate button press
    setMockPinState(LOW);
    button.aktualisieren();

    TEST_ASSERT_TRUE(button.istGedrueckt() == true);
    TEST_ASSERT_TRUE(button.wurdeGedrueckt() == true);
    TEST_ASSERT_TRUE(button.wurdeLangGedrueckt() == false);
}

void test_button_release() {
    EntprellterTaster button(2);

    // Simulate button press
    setMockPinState(LOW);
    button.aktualisieren();

    // Simulate button release
    setMockPinState(HIGH);
    button.aktualisieren();

    TEST_ASSERT_TRUE(button.istGedrueckt() == false);
    TEST_ASSERT_TRUE(button.wurdeGedrueckt() == false);
}

void test_long_press() {
    EntprellterTaster button(2);

    // Simulate button press
    setMockPinState(LOW);
    button.aktualisieren();

    // Simulate long press
    delay(2000); // Assuming longDruckDauer is set to 1000 ms
    button.aktualisieren();

    TEST_ASSERT_TRUE(button.wurdeLangGedrueckt() == true);
}

void setup() {
    UNITY_BEGIN(); // Start Unity test framework
    RUN_TEST(test_initial_state);
    RUN_TEST(test_button_press);
    RUN_TEST(test_button_release);
    RUN_TEST(test_long_press);
    UNITY_END(); // End Unity test framework
}

void loop() {
    // No need to implement anything here for the tests
}
