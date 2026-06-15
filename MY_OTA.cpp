#include <Arduino.h>
#include <ArduinoOTA.h>
#include "MY_OTA.h"

void setupOTA(const char* hostName, const char* otaPassword) {
  ArduinoOTA.setHostname(hostName);
  ArduinoOTA.setPassword(otaPassword);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA update started");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA update finished");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error [%u]\n", error);
  });

  ArduinoOTA.begin();

  Serial.println("OTA ready");
  Serial.print("OTA Hostname: ");
  Serial.println(hostName);
}

void handleOTA() {
  ArduinoOTA.handle();
}