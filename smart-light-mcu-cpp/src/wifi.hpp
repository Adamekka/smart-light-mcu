// Libs
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

// Configs
#include "wifi-config.hpp"

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  IPAddress localIP = WiFi.localIP();
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n",
                localIP[0], localIP[1], localIP[2], localIP[3]);
}
