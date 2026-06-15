#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

void storageBegin();

void saveLedState(
  uint8_t r,
  uint8_t g,
  uint8_t b,
  uint8_t brightness,
  unsigned long speed,
  int mode
);

void loadLedState(
  uint8_t &r,
  uint8_t &g,
  uint8_t &b,
  uint8_t &brightness,
  unsigned long &speed,
  int &mode
);

#endif