// Libs
#include <Arduino.h>

// Configs
#include "config.hpp"

// My libs
#include "wifi.hpp"

// You need to create your own `wifi-config.hpp` file
// with your own WiFi credentials
// See `wifi-config.hpp.example` for more info

class Light {
public:
  // Define the states
  enum State {
    ON,
    OFF,
  } state = OFF;

  // Turn the light on
  auto on() -> void {
    digitalWrite(LED_PIN, HIGH);
    state = ON;
  }

  // Turn the light off
  auto off() -> void {
    digitalWrite(LED_PIN, LOW);
    state = OFF;
  }
} Light;

auto pirInterrupt() -> void {
  // Check the state
  switch (Light.state) {
  case Light::State::ON:
    // Turn the light off
    Light.off();
    break;
  case Light::State::OFF:
    // Turn the light on
    Light.on();
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
  attachInterrupt(PIR_PIN, pirInterrupt, RISING);

  Serial.println("Setup done!");
}

auto loop() -> void {
  switch (Light.state) {
  case Light::State::ON:
    // Turn the light on
    Serial.printf("\33\rLight is on");
    delay(DELAY);
    break;
  case Light::State::OFF:
    // Turn the light off
    Serial.printf("\33\rLight is off");
    break;
  }
}
