// Libs
#include <Arduino.h>
#include "SinricPro.h"
#include "SinricProLight.h"

// Configs
#include "config.hpp"
#include "wifi-config.hpp"

// My libs
#include "wifi.hpp"

// You need to create your own `wifi-config.hpp` file
// with your own WiFi credentials and Smart Light credentials
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

bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state?"on":"off");
  Light.state = state?ON:OFF;
  return true; // request handled properly
}

bool onBrightness(const String &deviceId, int &brightness) {
  Light.brightness = brightness;
  Serial.printf("Device %s brightness level changed to %d\r\n", deviceId.c_str(), brightness);
  return true; // request handled properly
}

bool onAdjustBrightness(const String &deviceId, int brightnessDelta) {
  Light.brightness += brightnessDelta;
  Serial.printf("Device %s brightness level changed about %i to %d\r\n", deviceId.c_str(), brightnessDelta, Light.brightness);
  brightnessDelta = Light.brightness;
  return true; // request handled properly
}

void setupSinricPro() {
  // get a new Light device from SinricPro
  SinricProLight &myLight = SinricPro[LIGHT_ID];

  // set callback function to device
  myLight.onPowerState(onPowerState);
  myLight.onBrightness(onBrightness);
  myLight.onAdjustBrightness(onAdjustBrightness);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  // Initialize serial
  Serial.begin(115200);

  // Initialize WiFi
  setupWiFi();
  // Initialize SinricPro
  setupSinricPro();

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
