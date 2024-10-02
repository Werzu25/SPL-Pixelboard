#include <Arduino.h>

class DebouncedButton {
private:
    uint8_t pin;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;
    bool lastButtonState;
    bool buttonState;
    bool lastPressState;

public:
    DebouncedButton(uint8_t buttonPin, unsigned long debounceDelayMs = 50)
        : pin(buttonPin), debounceDelay(debounceDelayMs), lastDebounceTime(0), lastButtonState(HIGH), buttonState(HIGH), lastPressState(false) {
        pinMode(pin, INPUT_PULLUP);
    }

    void refresh() {
        bool reading = digitalRead(pin);
        if (reading != lastButtonState) {
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (reading != buttonState) {
                buttonState = reading;
            }
        }

        lastButtonState = reading;
    }

    bool isPressed() const {
        return !buttonState;
    }

    bool wasPressed() {
        bool wasPressed = (buttonState == LOW && lastPressState == false);
        lastPressState = (buttonState == LOW);
        return wasPressed;
    }
};