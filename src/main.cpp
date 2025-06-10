#include "HardwareSerial.h"
#include "info.hpp"
#include "pixelboard.hpp"
#include "snake.hpp"
#include "system.hpp"

#define LEDS1_PIN 25
#define LEDS2_PIN 26
#define JOYSTICK_BUTTON_PIN 32
#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 35
#define DHT_PIN 21
#define DHT_TYPE DHT22

#define PROJECT_ID "pixelboard-461909"
#define CLIENT_EMAIL "esp32-access@pixelboard-461909.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM =
    "-----BEGIN PRIVATE "
    "KEY-----"
    "\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC0jW8mfXVL5eR6\nTGaXKY"
    "Mvkhk1M1hex/"
    "qTpeVjOiHnWN7ihZ3cBFW5smhN65M409mvAZVQVeBGQBEj\niJcu5UdNmzp+"
    "LdB1fjvOfvp4x6LKXf2egH3JbZswfhIn1UJ3iDQCrOTvBt9Bkl0B\nV6/"
    "dVFVP3Ti436gs36Ryaw1qLZAskBu3n5Jpzo88dX3pGlmm4l5DI3DrywBjny/"
    "X\nc66pkdGp7kuh2n8nUBwYujDXXm0mKb48ZtfBti7d8wy506EBGQK6CoreQ/"
    "mR221d\nqEUfwvlA0hD6TLQ3ln3jI+"
    "UyGVbmiu9a4NiG3YInr59YcmSZe2wgRebKKl8dyL2m\nm9Wo59plAgMBAAECggEASg5tFcuFzh"
    "DIi0VFGyGC1oNFZaXezFmYNPFpyi5w85Rn\nL2CA5Fq+"
    "0sETuUo0A64Zbofmt6HRsN7caMfAwxJ2EaXm8xRVmauiWkuI7vtrkTfG\nLDol3fXgHp7HUaFO"
    "dvrd4m8KI+SctCeVkFRoJQ9PQsfJWUMblvRQXkH8qZ1wsmXi\n5pwO+"
    "nV33LLmJAyNOrsfGixP+0U0YT7qdEfkju1G6StQEhh6Vg9pFjzT6DGe+"
    "wsr\nZKNIlzVuySJZNBWRH+SKSKzEjS0jci45AurP0e/vKCbXgauhIEF68Mbw/"
    "JTy4X0J\nvaeY36jfEWJTst96hnq6j9mA8vOOPV50a5S7Qc5DbQKBgQD8UvsqHb/"
    "WIFABiLGm\nDvy4AokfCUasH7gknM8rjJ39ACuWzfwQrgRxa9OarL/"
    "zuOejnzYvqKbnsq+VMHR8\nV1DFSCOisJk0mgdLt2W8wQkg8SX94aEr6uIN+ZAxiD+"
    "Ck6vRAregbvZSUq6ggPHI\nu5Wwc1YwO3KXMGtMgVqfC57qFwKBgQC3LsmXRXj5JOG37WDLXuf"
    "GVYWa2RZAOT4y\nI9mFcEPlXO7HU+qAz5Sa/"
    "l18TPIxf9PYxUvhy7qfwqM42V7w9LwnL8LW50NbmPf5\n1J8rewqhrKFwrbEYlzEH2N+"
    "61GSNm587pqZtsF3SuAK/"
    "O5UqjC8I5U4aHIIADLWo\n5T8D1Ij44wKBgQCfjVVW52Lj4qDUp3w2FioFrUzJvFLdCl2lgSab"
    "Uhqg3deyT2RB\no9gZZGZ02yGx2s1WsJlR/"
    "6IyxtGyU+"
    "aEra3cYeZuvJ7UHUXZwglqFuJP20z7wg3p\nOGhjD9j3CUGnrvspej9lCmVl3vSfd4nsNXnNDt"
    "GEREMMiUiAIxeLlIyTVwKBgF/"
    "Y\nGR4O+XbSN0+"
    "24fhGldqOELOudhfwW3x8iWlqDfxKHZ0paP8wnT5jbKKG2AnSKhO3\nVzazE9hXTHOIOEa2pmO"
    "Sd81dfz7D0Ktk9ksXu75PxsBgANMu+DZ04MquKlZ1YGEj\nlty5dL22DanlEO0MH+"
    "f1omHDHLnN7JaMK+ZjI6+"
    "lAoGBAKFHLdjJqyXjG1Zqbb12\n9PCmjJ28inlHc1PPyeMpQC4Znpr0wZWz4Sk2hXn9meLaJdz"
    "Q2hKQ1PPCHiqECYWx\n/ZHPBxB3P/dyGYwrjyYi3JC//LQeoH74PpbrJh+3RVWS93u/"
    "0YCFvL+nr2BxDE9M\nl+MzYKYPFKvUA/+Z+ezlIwxw\n-----END PRIVATE KEY-----\n";
const char spreadsheetId[] = "15JXXrVG9KS5HSizQpAIbwQdSCG-wP7EQMFHO3kOD9HU";

const char *mqtt_user = "snake";
const char *mqtt_password = "tre]:7T\"gm:TZ5a";
const char *mqtt_host = "5cb42c9748844ca2bd6d0e57f6406124.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char *ssid = "pixel";
const char *password = "password";

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n\n[Setup] Starting initialization");

    PixelBoard *pixelboard = new PixelBoard(
        LEDS1_PIN, LEDS2_PIN, JOYSTICK_BUTTON_PIN, JOYSTICK_X_PIN,
        JOYSTICK_Y_PIN, ssid, password, vector<TaskHandle_t>(), {false, false},
        mqtt_user, mqtt_password, mqtt_port, mqtt_host, DHT_PIN, DHT_TYPE,
        PROJECT_ID, CLIENT_EMAIL, PRIVATE_KEY, spreadsheetId);

    TaskHandle_t SnakeHandle = NULL;
    TaskHandle_t InfoHandle = NULL;
    TaskHandle_t SystemHandle = NULL;
    vector<TaskHandle_t> tasks;

    Serial.println("[Setup] Creating tasks");

    xTaskCreate(Snake, "Snake", 20000, pixelboard, 1, &SnakeHandle);
    vTaskSuspend(SnakeHandle);
    delay(10);

    xTaskCreate(Info, "Info", 20000, pixelboard, 1, &InfoHandle);
    vTaskSuspend(InfoHandle);
    delay(10);

    tasks = {SnakeHandle, InfoHandle};
    pixelboard->tasks = tasks;

    xTaskCreate(System, "System", 20000, pixelboard, 1, &SystemHandle);

    pixelboard->display.clear();

    Serial.println("[Setup] Initialization complete");
}

void loop() {}
