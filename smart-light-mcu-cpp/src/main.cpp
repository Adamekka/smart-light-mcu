// Libs
#include <Arduino.h>

// Configs
#include "config.hpp"

// My libs
#include "wifi.hpp"

// You need to create your own `wifi-config.hpp` file
// with your own WiFi credentials
// See `wifi-config.hpp.example` for more info

// Define the states
enum LightState {
  ON,
  OFF
};

// Create light structure
struct Light {
  // The state
  LightState state = OFF;
  // The brightness
  int brightness = 0;
} Light;

void pirInterrupt() {
  // Check the state
  if (Light.state == ON) {
    // Turn the light off
    digitalWrite(LED_PIN, LOW);
    Light.state = OFF;
  } else {
    // Turn the light on
    digitalWrite(LED_PIN, HIGH);
    Light.state = ON;
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

  Serial.println("Setup done!");
}

void loop() {
  switch (Light.state) {
    case ON:
      // Turn the light on
      Serial.printf("\33\rLight is on");
      delay(DELAY);
      break;
    case OFF:
      // Turn the light off
      Serial.printf("\33\rLight is off");
      break;
  }
}
