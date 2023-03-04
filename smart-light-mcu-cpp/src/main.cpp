#include <Arduino.h>

#define PIR_PIN 19

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
}

void loop() {
  auto distance = digitalRead(PIR_PIN);

  Serial.println(distance);
  delay(1000);
}
