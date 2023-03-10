#include <Arduino.h>

// Define the pins
// All pins have interrupt capability
#define PIR_PIN 19

void setup() {
  // Initialize serial
  Serial.begin(115200);

  // Set the pins
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  // Read the distance
  auto distance = digitalRead(PIR_PIN);

  // Print the distance to the serial
  Serial.println(distance);

  // Wait for 1 second
  delay(1000);
}
