#include "button.hpp"
#include "unity.h"
#include <Arduino.h>

// NOTE: button is nit drin weil in src definiert wird und test sind nicht richtig aber geht fast

int simulatedButtonState = HIGH;

int mockDigitalRead(int pin) { return simulatedButtonState; }

EntprellterTaster *button;

void setUp(void) { button = new EntprellterTaster(2); }

void tearDown(void) { delete button; }

void test_button_initial_state(void) {
    TEST_ASSERT_TRUE(button->istGedrueckt() == false);
}

void test_button_press(void) {
    simulatedButtonState = LOW;
    button->aktualisieren();
    TEST_ASSERT_TRUE(button->istGedrueckt());

    simulatedButtonState = HIGH;
    button->aktualisieren();
    TEST_ASSERT_TRUE(button->istGedrueckt() == false);
}

void test_button_short_press_detection(void) {
    simulatedButtonState = LOW;
    button->aktualisieren();
    delay(50);
    button->aktualisieren();
    simulatedButtonState = HIGH;
    button->aktualisieren();

    TEST_ASSERT_TRUE(button->wurdeGedrueckt());
    TEST_ASSERT_FALSE(button->wurdeLangGedrueckt());
}

void test_button_long_press_detection(void) {
    simulatedButtonState = LOW;
    button->aktualisieren();
    delay(600);
    button->aktualisieren();
    simulatedButtonState = HIGH;
    button->aktualisieren();

    TEST_ASSERT_TRUE(button->wurdeLangGedrueckt());
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_button_initial_state);
    RUN_TEST(test_button_press);
    RUN_TEST(test_button_short_press_detection);
    RUN_TEST(test_button_long_press_detection);
    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}
