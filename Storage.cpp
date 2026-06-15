#include <Arduino.h>
#include <Preferences.h>
#include "Storage.h"

Preferences preferences;

void storageBegin() {
  preferences.begin("led_state", false);
}

void saveLedState(
  uint8_t r,
  uint8_t g,
  uint8_t b,
  uint8_t brightness,
  unsigned long speed,
  int mode
) {
  preferences.putUChar("r", r);
  preferences.putUChar("g", g);
  preferences.putUChar("b", b);
  preferences.putUChar("bright", brightness);
  preferences.putULong("speed", speed);
  preferences.putInt("mode", mode);

  Serial.println("LED state saved");
}

void loadLedState(
  uint8_t &r,
  uint8_t &g,
  uint8_t &b,
  uint8_t &brightness,
  unsigned long &speed,
  int &mode
) {
  r = preferences.getUChar("r", 255);
  g = preferences.getUChar("g", 0);
  b = preferences.getUChar("b", 0);
  brightness = preferences.getUChar("bright", 120);
  speed = preferences.getULong("speed", 300);
  mode = preferences.getInt("mode", 1);

  Serial.println("LED state loaded");
}