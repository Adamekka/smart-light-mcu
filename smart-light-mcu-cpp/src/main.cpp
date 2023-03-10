#include <Arduino.h>

// Define the states
enum LightState {
  ON,
  OFF
} lightState;

// Define the pins
// All pins have interrupt capability
#define LED_PIN 13
#define PIR_PIN 19

void pirInterrupt() {
  // Check the state
  if (lightState == ON) {
    // Turn the light off
    lightState = OFF;
  } else {
    // Turn the light on
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
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      break;
    case OFF:
      // Turn the light off
      Serial.println("Light is off");
      digitalWrite(LED_PIN, LOW);
      break;
  }
}
