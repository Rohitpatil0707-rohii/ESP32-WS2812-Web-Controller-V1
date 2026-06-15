#ifndef LED_H
#define LED_H

#include <Arduino.h>

void setupLed();

void ledOff();

void ledRed();
void ledGreen();
void ledBlue();
void ledWhite();
void ledYellow();
void ledCyan();
void ledMagenta();
void ledOrange();
void ledPurple();

void ledSetColor(uint8_t r, uint8_t g, uint8_t b);
void ledSetBrightness(uint8_t brightness);

void ledSolid();
void ledBlink();
void ledRainbow();
void ledBreathing();
void ledColorWipe();
void ledTheaterChase();
void ledSparkle();
void ledPolice();

void ledRunningLight();
void ledComet();
void ledMeteorRain();
void ledFireEffect();
void ledTwinkle();
void ledScanner();
void ledStrobe();
void ledRandomColor();
void ledRgbFade();
void ledRainbowCycle();
void ledColorBounce();
void ledSnake();

void ledBlinkSlow();
void ledBlinkFast();

void ledSpeedUp();
void ledSpeedDown();

unsigned long getLedSpeed();
uint8_t getLedBrightness();

void restoreLedState();
void saveCurrentLedState();

void handleLedButton();
void handleLedAnimation();

void ledStatusPowerOn();
void ledStatusWiFiConnecting();
void ledStatusWiFiConnected();
void ledStatusWiFiFailed();
void ledStatusWebClient();
void ledStatusLoginSuccess();
void ledStatusFactoryReset();

#endif