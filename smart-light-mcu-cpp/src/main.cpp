// Libs
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

// Configs
#include "config.hpp"
#include "wifi-config.hpp"

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

// Define a class for the light
class Light {
private:
  // Define a private function to handle HTTP results
  auto handleHttpResult(int httpCode) -> void {
    if (httpCode < 0) {
      // If the HTTP request failed, print an error message
      Serial.printf("[HTTP] GET... failed, error: %s\r\n",
                    http.errorToString(httpCode).c_str());
    } else {
      // If the HTTP request succeeded, print the response
      Serial.printf("[HTTP] GET... code: %d\r\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
  }

  // Define a private function to turn on the smart light
  auto smartLightOn() -> void {
    // Send the request to turn on the light
    Serial.println("[HTTP] begin...");
    int httpCode = http.begin(SMART_LIGHT_ON_URL);
    Serial.println("[HTTP] GET...");
    http.GET();
    // Handle the response
    handleHttpResult(httpCode);
    http.end();
  }

  // Define a private function to turn off the smart light
  auto smartLightOff() -> void {
    // Send the request to turn off the light
    Serial.println("[HTTP] begin...");
    int httpCode = http.begin(SMART_LIGHT_OFF_URL);
    Serial.println("[HTTP] GET...");
    http.GET();
    // Handle the response
    handleHttpResult(httpCode);
    http.end();
  }

public:
  // Define the states for the light
  enum State {
    ON,
    OFF,
  } state = OFF;

  bool activated = false;

  // Define a public function to turn the light on
  auto on() -> void {
    // Turn on the LED
    digitalWrite(LED_PIN, HIGH);
    // Turn on the smart light
    smartLightOn();
  }

  // Define a public function to turn the light off
  auto off() -> void {
    // Turn off the LED
    digitalWrite(LED_PIN, LOW);
    // Turn off the smart light
    smartLightOff();
  }
} light;

// Define a function to set up the WiFi connection
auto setupWiFi() -> void {
  // Print a message to indicate that the WiFi connection is being established
  Serial.printf("\r\n[Wifi]: Connecting");

  // Set the WiFi mode to station mode and connect to the network
  WiFi.mode(WIFI_STA);
  // Connect to the WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait for the connection to be established
  for (int i = 0; i < 60; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.printf(".");
      delay(250);
    } else {
      break;
    }
  }

  // If the connection was not established, print an error message
  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("\r[WiFi]: Failed to connect to WiFi\n");
    Serial.println("[WiFi]: Smart light will not work without WiFi connection");
  } else {
    IPAddress localIP = WiFi.localIP();
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n",
                  localIP[0], localIP[1], localIP[2], localIP[3]);
  }
}

auto pirInterrupt() -> void {
  // Check the state
  switch (light.state) {
  case Light::State::ON:
    light.activated = false;
    // Turn the light off
    light.state = Light::State::OFF;
    break;
  case Light::State::OFF:
    light.activated = false;
    // Turn the light on
    light.state = Light::State::ON;
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
  if (light.activated) {
    // Break loop
    return;
  }
  switch (light.state) {
  case Light::State::ON:
    // Turn the light on
    Serial.println("Light is on");
    light.on();
    delay(DELAY);
    light.activated = true;
    break;
  case Light::State::OFF:
    // Turn the light off
    Serial.println("Light is off");
    light.off();
    light.state = Light::State::OFF;
    light.activated = true;
    break;
  }
}
