#ifndef BUTTON_HPP
#define BUTTON_HPP

class Button {
  public:
    Button(int pin);
    virtual void update();
    bool isPressed() const;
    bool wasPressed();
    bool wasLongPressed();

  private:
    int buttonPin;
    bool buttonState;
    bool lastButtonState;
    bool lastPressState;
    unsigned long pressStartTime;
    unsigned long lastDebounceTime;
    bool longPressDetected;
    bool wasPressedFlag;
    bool wasLongPressedFlag;
    static const unsigned long debounceDelay = 50;
    static const unsigned long longPressDuration = 1000;
};

#endif
