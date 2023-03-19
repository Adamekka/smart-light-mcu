// Libs
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

// Configs
#include "wifi-config.hpp"

auto setupWiFi() -> void {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  for (int i = 0; i < 60; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.printf(".");
      delay(250);
    } else {
      break;
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("\r[WiFi]: Failed to connect to WiFi\n");
    Serial.println("[WiFi]: Smart light will not work without WiFi connection");
  } else {
    IPAddress localIP = WiFi.localIP();
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n",
                  localIP[0], localIP[1], localIP[2], localIP[3]);
  }
}
