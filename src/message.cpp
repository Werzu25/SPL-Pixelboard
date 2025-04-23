#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>

// LED Matrix configuration
#define LED_PIN      25  // Data pin for the matrix
#define COLOR_ORDER  GRB
#define CHIPSET      WS2812B

// Define matrix dimensions (single 32x8 display)
#define MATRIX_WIDTH   32
#define MATRIX_HEIGHT  8
#define BRIGHTNESS     50

// Total number of LEDs (32×8 = 256)
#define NUM_LEDS      256

// Create LED matrix for the display
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, VERTICAL_ZIGZAG_MATRIX> leds;

// Create scrolling text object
cLEDText ScrollingMsg;

// Scrolling speed (milliseconds between updates)
#define SCROLL_SPEED  50

// Message to display
const unsigned char TxtDemo[] = { EFFECT_SCROLL_LEFT "      Julian du schwuler Hund      " };

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("LED Matrix Initializing...");
  
    // Initialize FastLED for the matrix
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
    
    // Setup text scrolling
    ScrollingMsg.SetFont(MatriseFontData);
    ScrollingMsg.Init(&leds, leds.Width(), leds.Height(), 0, 0);
    ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
    ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_RGB, 138, 43, 226);
    
    Serial.println("LED Matrix Initialized");
}

void loop() {
    if (ScrollingMsg.UpdateText() == -1) {
        // Text scrolling is complete, restart with the same text
        ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
    }
    else {
        // Simply show the updated display and wait
        FastLED.show();
        delay(SCROLL_SPEED);
    }
}