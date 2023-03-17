// Libs
#include <Arduino.h>

// Configs
#include "config.hpp"

// My libs
#include "wifi.hpp"

// You need to create your own `wifi-config.hpp` file
// with your own WiFi credentials and Smart Light credentials
// See `wifi-config.hpp.example` for more info

// Define the states
enum LightState {
  ON,
  OFF
} lightState;

void pirInterrupt() {
  // Check the state
  if (lightState == ON) {
    // Turn the light off
    digitalWrite(LED_PIN, LOW);
    lightState = OFF;
  } else {
    // Turn the light on
    digitalWrite(LED_PIN, HIGH);
    lightState = ON;
  }
}

void setup() {
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
}

void loop() {
  switch (lightState) {
    case ON:
      // Turn the light on
      Serial.println("Light is on");
      delay(DELAY);
      break;
    case OFF:
      // Turn the light off
      Serial.println("Light is off");
      break;
  }
}
