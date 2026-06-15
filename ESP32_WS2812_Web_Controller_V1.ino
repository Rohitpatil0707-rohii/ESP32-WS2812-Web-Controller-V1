#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "Auth.h"
#include "MY_OTA.h"
#include "WebPage.h"
#include "LED.h"
#include "Storage.h"
#include "Config.h"

WebServer server(80);

String otaHostName;
String otaPassword;

bool checkLogin() {
  return requireLogin(server);
}

void handleRoot() {
  if (isConfigMode()) {
      ledStatusWebClient();
    server.send(200, "text/html; charset=utf-8", getConfigPageHtml());
    return;
  }

  if (!checkLogin()) {
    return;
  }
ledStatusWebClient();
  server.send_P(200, "text/html; charset=utf-8", MAIN_page);
}

void handlePing() {
  server.send(200, "text/plain", "pong");
}

void handleOn() {
  if (!checkLogin()) return;
  ledWhite();
  server.send(200, "text/plain", "WHITE");
}

void handleOff() {
  if (!checkLogin()) return;
  ledOff();
  server.send(200, "text/plain", "OFF");
}

void handleRed() {
  if (!checkLogin()) return;
  ledRed();
  server.send(200, "text/plain", "RED");
}

void handleGreen() {
  if (!checkLogin()) return;
  ledGreen();
  server.send(200, "text/plain", "GREEN");
}

void handleBlue() {
  if (!checkLogin()) return;
  ledBlue();
  server.send(200, "text/plain", "BLUE");
}

void handleYellow() {
  if (!checkLogin()) return;
  ledYellow();
  server.send(200, "text/plain", "YELLOW");
}

void handleCyan() {
  if (!checkLogin()) return;
  ledCyan();
  server.send(200, "text/plain", "CYAN");
}

void handleMagenta() {
  if (!checkLogin()) return;
  ledMagenta();
  server.send(200, "text/plain", "MAGENTA");
}

void handleOrange() {
  if (!checkLogin()) return;
  ledOrange();
  server.send(200, "text/plain", "ORANGE");
}

void handlePurple() {
  if (!checkLogin()) return;
  ledPurple();
  server.send(200, "text/plain", "PURPLE");
}

void handleSolid() {
  if (!checkLogin()) return;
  ledSolid();
  server.send(200, "text/plain", "SOLID");
}

void handleBlink() {
  if (!checkLogin()) return;
  ledBlink();
  server.send(200, "text/plain", "BLINK");
}

void handleRainbow() {
  if (!checkLogin()) return;
  ledRainbow();
  server.send(200, "text/plain", "RAINBOW");
}

void handleBreathing() {
  if (!checkLogin()) return;
  ledBreathing();
  server.send(200, "text/plain", "BREATHING");
}

void handleColorWipe() {
  if (!checkLogin()) return;
  ledColorWipe();
  server.send(200, "text/plain", "COLOR WIPE");
}

void handleTheater() {
  if (!checkLogin()) return;
  ledTheaterChase();
  server.send(200, "text/plain", "THEATER");
}

void handleSparkle() {
  if (!checkLogin()) return;
  ledSparkle();
  server.send(200, "text/plain", "SPARKLE");
}

void handlePolice() {
  if (!checkLogin()) return;
  ledPolice();
  server.send(200, "text/plain", "POLICE");
}

void handleRunningLight() {
  if (!checkLogin()) return;
  ledRunningLight();
  server.send(200, "text/plain", "RUNNING LIGHT");
}

void handleComet() {
  if (!checkLogin()) return;
  ledComet();
  server.send(200, "text/plain", "COMET");
}

void handleMeteorRain() {
  if (!checkLogin()) return;
  ledMeteorRain();
  server.send(200, "text/plain", "METEOR RAIN");
}

void handleFireEffect() {
  if (!checkLogin()) return;
  ledFireEffect();
  server.send(200, "text/plain", "FIRE EFFECT");
}

void handleTwinkle() {
  if (!checkLogin()) return;
  ledTwinkle();
  server.send(200, "text/plain", "TWINKLE");
}

void handleScanner() {
  if (!checkLogin()) return;
  ledScanner();
  server.send(200, "text/plain", "SCANNER");
}

void handleStrobe() {
  if (!checkLogin()) return;
  ledStrobe();
  server.send(200, "text/plain", "STROBE");
}

void handleRandomColor() {
  if (!checkLogin()) return;
  ledRandomColor();
  server.send(200, "text/plain", "RANDOM COLOR");
}

void handleRgbFade() {
  if (!checkLogin()) return;
  ledRgbFade();
  server.send(200, "text/plain", "RGB FADE");
}

void handleRainbowCycle() {
  if (!checkLogin()) return;
  ledRainbowCycle();
  server.send(200, "text/plain", "RAINBOW CYCLE");
}

void handleColorBounce() {
  if (!checkLogin()) return;
  ledColorBounce();
  server.send(200, "text/plain", "COLOR BOUNCE");
}

void handleSnake() {
  if (!checkLogin()) return;
  ledSnake();
  server.send(200, "text/plain", "SNAKE");
}

void handleSpeedUp() {
  if (!checkLogin()) return;
  ledSpeedUp();

  String msg = "Speed: " + String(getLedSpeed()) + " ms";
  server.send(200, "text/plain", msg);
}

void handleSpeedDown() {
  if (!checkLogin()) return;
  ledSpeedDown();

  String msg = "Speed: " + String(getLedSpeed()) + " ms";
  server.send(200, "text/plain", msg);
}

void handleBlinkSlow() {
  if (!checkLogin()) return;
  ledBlinkSlow();

  String msg = "Speed: " + String(getLedSpeed()) + " ms";
  server.send(200, "text/plain", msg);
}

void handleBlinkFast() {
  if (!checkLogin()) return;
  ledBlinkFast();

  String msg = "Speed: " + String(getLedSpeed()) + " ms";
  server.send(200, "text/plain", msg);
}

void handleSpeedStatus() {
  if (!checkLogin()) return;

  String msg = String(getLedSpeed()) + " ms";
  server.send(200, "text/plain", msg);
}

void handleBrightnessStatus() {
  if (!checkLogin()) return;

  String msg = String(getLedBrightness());
  server.send(200, "text/plain", msg);
}

void handleBrightness() {
  if (!checkLogin()) return;

  if (server.hasArg("value")) {
    int brightness = server.arg("value").toInt();

    brightness = constrain(brightness, 0, 255);

    ledSetBrightness((uint8_t)brightness);

    String msg = "Brightness: " + String(brightness);
    server.send(200, "text/plain", msg);
  } else {
    server.send(400, "text/plain", "Missing brightness value");
  }
}

void handleColor() {
  if (!checkLogin()) return;

  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();

    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    ledSetColor((uint8_t)r, (uint8_t)g, (uint8_t)b);

    String msg = "Color: R=" + String(r) + " G=" + String(g) + " B=" + String(b);
    server.send(200, "text/plain", msg);
  } else {
    server.send(400, "text/plain", "Missing RGB value");
  }
}

void setupWebServer() {
  setupAuthRoutes(server);

  server.on("/ping", handlePing);
  server.on("/", handleRoot);

  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.on("/red", handleRed);
  server.on("/green", handleGreen);
  server.on("/blue", handleBlue);
  server.on("/yellow", handleYellow);
  server.on("/cyan", handleCyan);
  server.on("/magenta", handleMagenta);
  server.on("/orange", handleOrange);
  server.on("/purple", handlePurple);

  server.on("/solid", handleSolid);
  server.on("/blink", handleBlink);
  server.on("/rainbow", handleRainbow);
  server.on("/breathing", handleBreathing);
  server.on("/colorwipe", handleColorWipe);
  server.on("/theater", handleTheater);
  server.on("/sparkle", handleSparkle);
  server.on("/police", handlePolice);

  server.on("/running", handleRunningLight);
  server.on("/comet", handleComet);
  server.on("/meteor", handleMeteorRain);
  server.on("/fire", handleFireEffect);
  server.on("/twinkle", handleTwinkle);
  server.on("/scanner", handleScanner);
  server.on("/strobe", handleStrobe);
  server.on("/randomcolor", handleRandomColor);
  server.on("/rgbfade", handleRgbFade);
  server.on("/rainbowcycle", handleRainbowCycle);
  server.on("/colorbounce", handleColorBounce);
  server.on("/snake", handleSnake);

  server.on("/speedup", handleSpeedUp);
  server.on("/speeddown", handleSpeedDown);
  server.on("/blinkslow", handleBlinkSlow);
  server.on("/blinkfast", handleBlinkFast);
  server.on("/speed", handleSpeedStatus);

  server.on("/brightness", handleBrightness);
  server.on("/brightnessstatus", handleBrightnessStatus);
  server.on("/color", handleColor);

  setupConfigRoutes(server);

  server.begin();
  Serial.println("Web server started");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  storageBegin();
  configBegin();

  setupLed();
  ledStatusPowerOn();
  restoreLedState();

  bool wifiConnected = connectWiFiFromConfig(15000);

  if (wifiConnected) {
  ledStatusWiFiConnected();

    otaHostName = getDeviceName();
    otaPassword = getOtaPassword();

    if (MDNS.begin(otaHostName.c_str())) {
      Serial.print("mDNS started. Open: http://");
      Serial.print(otaHostName);
      Serial.println(".local");
    } else {

      Serial.println("mDNS failed");
    }

    setupOTA(otaHostName.c_str(), otaPassword.c_str());
  } else {
    ledStatusWiFiFailed();
    Serial.println("OTA disabled in setup AP mode");
  }

  setupWebServer();

  Serial.println("System ready");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    handleOTA();
  }

  server.handleClient();
  handleLedButton();
  handleLedAnimation();
}