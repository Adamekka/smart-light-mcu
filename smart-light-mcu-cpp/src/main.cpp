// Libs
#include <Arduino.h>
#include <HTTPClient.h>

// Configs
#include "config.hpp"

// My libs
#include "wifi.hpp"

// You need to create your own `wifi-config.hpp` file
// with your own WiFi credentials
// See `wifi-config.hpp.example` for more info

// Global variables
HTTPClient http;

// Define macros
#define SMART_LIGHT_ON_URL                                                     \
  "https://maker.ifttt.com/trigger/light_on/with/key/" IFTTT_KEY
#define SMART_LIGHT_OFF_URL                                                    \
  "https://maker.ifttt.com/trigger/light_off/with/key/" IFTTT_KEY

class Light {
private:
  auto smartLightOn() -> void {
    // Send the request
    Serial.println("[HTTP] begin...");
    int httpCode = http.begin(SMART_LIGHT_ON_URL);
    Serial.println("[HTTP] GET...");
    http.GET();

    if (httpCode < 0) {
      Serial.printf("[HTTP] GET... failed, error: %s\r\n",
                    http.errorToString(httpCode).c_str());
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\r\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }

    http.end();
  }

  auto smartLightOff() -> void {
    // Send the request
    Serial.println("[HTTP] begin...");
    int httpCode = http.begin(SMART_LIGHT_OFF_URL);
    Serial.println("[HTTP] GET...");
    http.GET();

    if (httpCode < 0) {
      Serial.printf("[HTTP] GET... failed, error: %s\r\n",
                    http.errorToString(httpCode).c_str());
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\r\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }

    http.end();
  }

public:
  // Define the states
  enum State {
    ON,
    OFF,
  } state = OFF;

  bool activated = false;

  // Turn the light on
  auto on() -> void {
    // Turn on the LED
    digitalWrite(LED_PIN, HIGH);
    // Turn on the smart light
    smartLightOn();
  }

  // Turn the light off
  auto off() -> void {
    // Turn off the LED
    digitalWrite(LED_PIN, LOW);
    // Turn off the smart light
    smartLightOff();
  }
} Light;

auto pirInterrupt() -> void {
  // Check the state
  switch (Light.state) {
  case Light::State::ON:
    Light.activated = false;
    // Turn the light off
    Light.state = Light::State::OFF;
    break;
  case Light::State::OFF:
    Light.activated = false;
    // Turn the light on
    Light.state = Light::State::ON;
    break;
  }
}

auto setup() -> void {

  // Initialize serial
  Serial.begin(115200);

  // Initialize WiFi
  setupWiFi();

  // Set the pins
  // Inputs
  pinMode(PIR_PIN, INPUT);
  // Outputs
  pinMode(LED_PIN, OUTPUT);

  // Attach the interrupt
  attachInterrupt(PIR_PIN, pirInterrupt, CHANGE);

  Serial.println("Setup done!");
}

auto loop() -> void {
  if (Light.activated) {
    // Break loop
    return;
  }
  switch (Light.state) {
  case Light::State::ON:
    // Turn the light on
    Serial.println("Light is on");
    Light.on();
    delay(DELAY);
    Light.activated = true;
    break;
  case Light::State::OFF:
    // Turn the light off
    Serial.println("Light is off");
    Light.off();
    Light.state = Light::State::OFF;
    Light.activated = true;
    break;
  }
}
