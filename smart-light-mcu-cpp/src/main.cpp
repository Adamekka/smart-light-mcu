#include <Arduino.h>
#include "config.hpp"

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
