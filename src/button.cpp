#include "button.hpp"
#include <Arduino.h>

Button::Button(int pin) {
    buttonPin = pin;
    pinMode(buttonPin, INPUT_PULLUP);
    buttonState = HIGH;
    lastButtonState = HIGH;
    lastPressState = false;
    pressStartTime = 0;
    lastDebounceTime = 0;
    longPressDetected = false;
    wasPressedFlag = false;
    wasLongPressedFlag = false;
}

void Button::update() {
    bool currentState = digitalRead(buttonPin);

    if (currentState != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (currentState != buttonState) {
            buttonState = currentState;

            if (buttonState == LOW) {
                pressStartTime = millis();
                longPressDetected = false;
            } else {
                unsigned long pressDuration = millis() - pressStartTime;
                if (pressDuration >= longPressDuration && !longPressDetected) {
                    wasLongPressedFlag = true;
                    longPressDetected = true;
                } else if (pressDuration < longPressDuration) {
                    wasPressedFlag = true;
                }
                pressStartTime = 0;
            }
        }
    }

    if (buttonState == LOW && !longPressDetected) {
        if ((millis() - pressStartTime) >= longPressDuration) {
            wasLongPressedFlag = true;
            longPressDetected = true;
        }
    }

    lastButtonState = currentState;
}

bool Button::isPressed() const { return buttonState == LOW; }

bool Button::wasPressed() {
    if (wasPressedFlag) {
        wasPressedFlag = false;
        return true;
    }
    return false;
}

bool Button::wasLongPressed() {
    if (wasLongPressedFlag) {
        wasLongPressedFlag = false;
        return true;
    }
    return false;
}
