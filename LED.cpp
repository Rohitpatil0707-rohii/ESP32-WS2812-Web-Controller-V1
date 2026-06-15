#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LED.h"
#include "Storage.h"
#include "Config.h"

#define LED_PIN     27
#define NUM_LEDS    150
#define BUTTON_PIN  25

#define SPEED_MIN   100
#define SPEED_MAX   1500
#define SPEED_STEP  100

#define MODE_OFF             0
#define MODE_SOLID           1
#define MODE_BLINK           2
#define MODE_RAINBOW         3
#define MODE_BREATHING       4
#define MODE_COLOR_WIPE      5
#define MODE_THEATER         6
#define MODE_SPARKLE         7
#define MODE_POLICE          8
#define MODE_RAINBOW_BLINK   9

#define MODE_RUNNING_LIGHT   10
#define MODE_COMET           11
#define MODE_METEOR_RAIN     12
#define MODE_FIRE_EFFECT     13
#define MODE_TWINKLE         14
#define MODE_SCANNER         15
#define MODE_STROBE          16
#define MODE_RANDOM_COLOR    17
#define MODE_RGB_FADE        18
#define MODE_RAINBOW_CYCLE   19
#define MODE_COLOR_BOUNCE    20
#define MODE_SNAKE           21

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentMode = MODE_SOLID;

int buttonMode = 0;
bool lastButtonState = HIGH;

unsigned long lastEffectTime = 0;
unsigned long effectSpeed = 300;

uint16_t rainbowHue = 0;

uint8_t currentR = 255;
uint8_t currentG = 255;
uint8_t currentB = 255;
uint8_t currentBrightness = 120;

bool blinkState = false;

int breathValue = 0;
int breathDirection = 5;

int wipeIndex = 0;
int theaterOffset = 0;

bool policeState = false;
bool restoringState = false;

// New animation variables
int runningIndex = 0;

int cometIndex = 0;

int meteorIndex = 0;

byte fireHeat[NUM_LEDS];

int scannerIndex = 0;
int scannerDirection = 1;

bool strobeState = false;

uint8_t fadeR = 255;
uint8_t fadeG = 0;
uint8_t fadeB = 0;
int fadeStage = 0;

int bounceIndex = 0;
int bounceDirection = 1;

int snakeHead = 0;


// ---------- BASIC LED FUNCTIONS ----------

void showColor(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }

  pixels.show();
}

void showOff() {
  pixels.clear();

  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }

  pixels.show();
  delay(5);
}

void showCurrentColor() {
  showColor(currentR, currentG, currentB);
}

void setCurrentColor(uint8_t red, uint8_t green, uint8_t blue) {
  currentR = red;
  currentG = green;
  currentB = blue;
}

void showRainbowFrame() {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint16_t pixelHue = rainbowHue + (i * 65536L / NUM_LEDS);
    pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  }

  pixels.show();
  rainbowHue += 512;
}

void fadeAllPixels(uint8_t amount) {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t color = pixels.getPixelColor(i);

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    r = (r <= amount) ? 0 : r - amount;
    g = (g <= amount) ? 0 : g - amount;
    b = (b <= amount) ? 0 : b - amount;

    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
}

void ledStatusFlash(uint8_t r, uint8_t g, uint8_t b, int times, int onTime, int offTime) {
  int oldMode = currentMode;
  uint8_t oldBrightness = currentBrightness;

  pixels.setBrightness(120);

  for (int t = 0; t < times; t++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
    delay(onTime);

    pixels.clear();
    pixels.show();
    delay(offTime);
  }

  pixels.setBrightness(oldBrightness);

  currentMode = oldMode;

  if (currentMode == MODE_SOLID) {
    showCurrentColor();
  } else if (currentMode == MODE_OFF) {
    showOff();
  }
}

void ledStatusPowerOn() {
  ledStatusFlash(255, 255, 255, 1, 250, 150);
}

void ledStatusWiFiConnecting() {
  ledStatusFlash(0, 0, 255, 1, 120, 120);
}

void ledStatusWiFiConnected() {
  ledStatusFlash(0, 255, 0, 3, 180, 150);
}

void ledStatusWiFiFailed() {
  ledStatusFlash(255, 0, 0, 5, 180, 180);
}

void ledStatusWebClient() {
  ledStatusFlash(0, 255, 255, 2, 120, 120);
}

void ledStatusLoginSuccess() {
  ledStatusFlash(150, 0, 255, 2, 150, 120);
}


// ---------- SAVE / RESTORE ----------

void saveCurrentLedState() {
  if (restoringState == true) {
    return;
  }

  saveLedState(
    currentR,
    currentG,
    currentB,
    currentBrightness,
    effectSpeed,
    currentMode
  );
}

void restoreLedState() {
  restoringState = true;

  loadLedState(
    currentR,
    currentG,
    currentB,
    currentBrightness,
    effectSpeed,
    currentMode
  );

  pixels.setBrightness(currentBrightness);

  if (currentMode == MODE_OFF) {
    showOff();
  } else if (currentMode == MODE_SOLID) {
    showCurrentColor();
  } else if (currentMode == MODE_BLINK) {
    blinkState = false;
  } else if (currentMode == MODE_RAINBOW) {
    rainbowHue = 0;
  } else if (currentMode == MODE_RAINBOW_BLINK) {
    rainbowHue = 0;
    blinkState = false;
  } else if (currentMode == MODE_BREATHING) {
    breathValue = 0;
    breathDirection = 5;
  } else if (currentMode == MODE_COLOR_WIPE) {
    wipeIndex = 0;
    pixels.clear();
    pixels.show();
  } else if (currentMode == MODE_THEATER) {
    theaterOffset = 0;
  } else if (currentMode == MODE_SPARKLE) {
    pixels.clear();
    pixels.show();
  } else if (currentMode == MODE_POLICE) {
    policeState = false;
  } else if (currentMode == MODE_RUNNING_LIGHT) {
    runningIndex = 0;
  } else if (currentMode == MODE_COMET) {
    cometIndex = 0;
    pixels.clear();
    pixels.show();
  } else if (currentMode == MODE_METEOR_RAIN) {
    meteorIndex = 0;
    pixels.clear();
    pixels.show();
  } else if (currentMode == MODE_FIRE_EFFECT) {
    for (int i = 0; i < NUM_LEDS; i++) {
      fireHeat[i] = 0;
    }
  } else if (currentMode == MODE_TWINKLE) {
    pixels.clear();
    pixels.show();
  } else if (currentMode == MODE_SCANNER) {
    scannerIndex = 0;
    scannerDirection = 1;
  } else if (currentMode == MODE_STROBE) {
    strobeState = false;
  } else if (currentMode == MODE_RANDOM_COLOR) {
    pixels.clear();
    pixels.show();
  } else if (currentMode == MODE_RGB_FADE) {
    fadeR = 255;
    fadeG = 0;
    fadeB = 0;
    fadeStage = 0;
  } else if (currentMode == MODE_RAINBOW_CYCLE) {
    rainbowHue = 0;
  } else if (currentMode == MODE_COLOR_BOUNCE) {
    bounceIndex = 0;
    bounceDirection = 1;
  } else if (currentMode == MODE_SNAKE) {
    snakeHead = 0;
  } else {
    currentMode = MODE_SOLID;
    showCurrentColor();
  }

  restoringState = false;

  Serial.println("LED state restored");
}


// ---------- SETUP ----------

void setupLed() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixels.begin();
  pixels.setBrightness(currentBrightness);
  pixels.clear();
  pixels.show();

  randomSeed(analogRead(0));

  Serial.println("WS2812 ready on GPIO27");
  Serial.println("Button ready on GPIO25");
}


// ---------- BASIC MODES ----------

void ledOff() {
  currentMode = MODE_OFF;
  blinkState = false;
  strobeState = false;
  policeState = false;

  pixels.setBrightness(0);
  pixels.clear();

  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }

  pixels.show();
  delay(10);

  pixels.setBrightness(currentBrightness);

  saveCurrentLedState();

  Serial.println("MODE OFF");
}
void ledSolid() {
  currentMode = MODE_SOLID;
  showCurrentColor();
  saveCurrentLedState();

  Serial.println("MODE SOLID");
}

void ledBlink() {
  if (currentMode == MODE_RAINBOW || currentMode == MODE_RAINBOW_BLINK) {
    currentMode = MODE_RAINBOW_BLINK;
    blinkState = false;
    saveCurrentLedState();

    Serial.println("MODE RAINBOW BLINK");
    return;
  }

  currentMode = MODE_BLINK;
  blinkState = false;
  saveCurrentLedState();

  Serial.println("MODE BLINK");
}

void ledRainbow() {
  currentMode = MODE_RAINBOW;
  rainbowHue = 0;
  saveCurrentLedState();

  Serial.println("MODE RAINBOW");
}

void ledBreathing() {
  currentMode = MODE_BREATHING;
  breathValue = 0;
  breathDirection = 5;
  saveCurrentLedState();

  Serial.println("MODE BREATHING");
}

void ledColorWipe() {
  currentMode = MODE_COLOR_WIPE;
  wipeIndex = 0;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE COLOR WIPE");
}

void ledTheaterChase() {
  currentMode = MODE_THEATER;
  theaterOffset = 0;
  saveCurrentLedState();

  Serial.println("MODE THEATER CHASE");
}

void ledSparkle() {
  currentMode = MODE_SPARKLE;
  saveCurrentLedState();

  Serial.println("MODE SPARKLE");
}

void ledPolice() {
  currentMode = MODE_POLICE;
  policeState = false;
  saveCurrentLedState();

  Serial.println("MODE POLICE");
}


// ---------- NEW ANIMATION MODE FUNCTIONS ----------

void ledRunningLight() {
  currentMode = MODE_RUNNING_LIGHT;
  runningIndex = 0;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE RUNNING LIGHT");
}

void ledComet() {
  currentMode = MODE_COMET;
  cometIndex = 0;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE COMET");
}

void ledMeteorRain() {
  currentMode = MODE_METEOR_RAIN;
  meteorIndex = 0;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE METEOR RAIN");
}

void ledFireEffect() {
  currentMode = MODE_FIRE_EFFECT;

  for (int i = 0; i < NUM_LEDS; i++) {
    fireHeat[i] = 0;
  }

  saveCurrentLedState();

  Serial.println("MODE FIRE EFFECT");
}

void ledTwinkle() {
  currentMode = MODE_TWINKLE;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE TWINKLE");
}

void ledScanner() {
  currentMode = MODE_SCANNER;
  scannerIndex = 0;
  scannerDirection = 1;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE SCANNER / KNIGHT RIDER");
}

void ledStrobe() {
  currentMode = MODE_STROBE;
  strobeState = false;
  saveCurrentLedState();

  Serial.println("MODE STROBE");
}

void ledRandomColor() {
  currentMode = MODE_RANDOM_COLOR;
  saveCurrentLedState();

  Serial.println("MODE RANDOM COLOR");
}

void ledRgbFade() {
  currentMode = MODE_RGB_FADE;
  fadeR = 255;
  fadeG = 0;
  fadeB = 0;
  fadeStage = 0;
  saveCurrentLedState();

  Serial.println("MODE RGB FADE");
}

void ledRainbowCycle() {
  currentMode = MODE_RAINBOW_CYCLE;
  rainbowHue = 0;
  saveCurrentLedState();

  Serial.println("MODE RAINBOW CYCLE");
}

void ledColorBounce() {
  currentMode = MODE_COLOR_BOUNCE;
  bounceIndex = 0;
  bounceDirection = 1;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE COLOR BOUNCE");
}

void ledSnake() {
  currentMode = MODE_SNAKE;
  snakeHead = 0;
  pixels.clear();
  pixels.show();
  saveCurrentLedState();

  Serial.println("MODE SNAKE");
}


// ---------- COLOR FUNCTIONS ----------

void ledRed() {
  setCurrentColor(255, 0, 0);
  ledSolid();

  Serial.println("LED RED");
}

void ledGreen() {
  setCurrentColor(0, 255, 0);
  ledSolid();

  Serial.println("LED GREEN");
}

void ledBlue() {
  setCurrentColor(0, 0, 255);
  ledSolid();

  Serial.println("LED BLUE");
}

void ledWhite() {
  setCurrentColor(255, 255, 255);
  ledSolid();

  Serial.println("LED WHITE");
}

void ledYellow() {
  setCurrentColor(255, 255, 0);
  ledSolid();

  Serial.println("LED YELLOW");
}

void ledCyan() {
  setCurrentColor(0, 255, 255);
  ledSolid();

  Serial.println("LED CYAN");
}

void ledMagenta() {
  setCurrentColor(255, 0, 255);
  ledSolid();

  Serial.println("LED MAGENTA");
}

void ledOrange() {
  setCurrentColor(255, 80, 0);
  ledSolid();

  Serial.println("LED ORANGE");
}

void ledPurple() {
  setCurrentColor(120, 0, 255);
  ledSolid();

  Serial.println("LED PURPLE");
}

void ledSetColor(uint8_t r, uint8_t g, uint8_t b) {
  setCurrentColor(r, g, b);
  ledSolid();

  Serial.print("Custom color: R=");
  Serial.print(r);
  Serial.print(" G=");
  Serial.print(g);
  Serial.print(" B=");
  Serial.println(b);
}


// ---------- BRIGHTNESS ----------

void ledSetBrightness(uint8_t brightness) {
  currentBrightness = brightness;
  pixels.setBrightness(currentBrightness);

  if (currentMode == MODE_OFF) {
    showOff();
  } else if (currentMode == MODE_SOLID) {
    showCurrentColor();
  } else {
    pixels.show();
  }

  saveCurrentLedState();

  Serial.print("Brightness: ");
  Serial.println(currentBrightness);
}


// ---------- SPEED / BLINK ----------

void ledBlinkSlow() {
  if (currentMode == MODE_RAINBOW || currentMode == MODE_RAINBOW_BLINK) {
    currentMode = MODE_RAINBOW_BLINK;
  } else {
    currentMode = MODE_BLINK;
  }

  effectSpeed = 500;
  blinkState = false;
  saveCurrentLedState();

  Serial.println("SLOW BLINK");
}

void ledBlinkFast() {
  if (currentMode == MODE_RAINBOW || currentMode == MODE_RAINBOW_BLINK) {
    currentMode = MODE_RAINBOW_BLINK;
  } else {
    currentMode = MODE_BLINK;
  }

  effectSpeed = 100;
  blinkState = false;
  saveCurrentLedState();

  Serial.println("FAST BLINK");
}

void ledSpeedUp() {
  if (effectSpeed < SPEED_MAX) {
    effectSpeed += SPEED_STEP;
  }

  saveCurrentLedState();

  Serial.print("Speed +: ");
  Serial.print(effectSpeed);
  Serial.println(" ms");
}

void ledSpeedDown() {
  if (effectSpeed > SPEED_MIN) {
    effectSpeed -= SPEED_STEP;
  }

  saveCurrentLedState();

  Serial.print("Speed -: ");
  Serial.print(effectSpeed);
  Serial.println(" ms");
}

unsigned long getLedSpeed() {
  return effectSpeed;
}

uint8_t getLedBrightness() {
  return currentBrightness;
}


// ---------- OLD ANIMATION RUNNERS ----------

void runBlink() {
  blinkState = !blinkState;

  if (blinkState) {
    showCurrentColor();
  } else {
    showOff();
  }
}

void runRainbowBlink() {
  blinkState = !blinkState;

  if (blinkState) {
    showRainbowFrame();
  } else {
    showOff();
  }
}

void runBreathing() {
  pixels.setBrightness(breathValue);

  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(currentR, currentG, currentB));
  }

  pixels.show();

  breathValue += breathDirection;

  if (breathValue >= currentBrightness) {
    breathValue = currentBrightness;
    breathDirection = -5;
  }

  if (breathValue <= 0) {
    breathValue = 0;
    breathDirection = 5;
  }

  pixels.setBrightness(currentBrightness);
}

void runColorWipe() {
  if (wipeIndex >= NUM_LEDS) {
    wipeIndex = 0;
    pixels.clear();
  }

  pixels.setPixelColor(wipeIndex, pixels.Color(currentR, currentG, currentB));
  pixels.show();

  wipeIndex++;
}

void runTheaterChase() {
  pixels.clear();

  for (int i = theaterOffset; i < NUM_LEDS; i += 3) {
    pixels.setPixelColor(i, pixels.Color(currentR, currentG, currentB));
  }

  pixels.show();

  theaterOffset++;

  if (theaterOffset > 2) {
    theaterOffset = 0;
  }
}

void runSparkle() {
  pixels.clear();

  int index = random(NUM_LEDS);

  pixels.setPixelColor(index, pixels.Color(currentR, currentG, currentB));
  pixels.show();
}

void runPolice() {
  policeState = !policeState;

  if (policeState) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i < NUM_LEDS / 2) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      } else {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      }
    }
  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i < NUM_LEDS / 2) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      } else {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      }
    }
  }

  pixels.show();
}


// ---------- NEW ANIMATION RUNNERS ----------

void runRunningLight() {
  pixels.clear();
  pixels.setPixelColor(runningIndex, pixels.Color(currentR, currentG, currentB));
  pixels.show();

  runningIndex++;

  if (runningIndex >= NUM_LEDS) {
    runningIndex = 0;
  }
}

void runComet() {
  fadeAllPixels(45);

  pixels.setPixelColor(cometIndex, pixels.Color(currentR, currentG, currentB));
  pixels.show();

  cometIndex++;

  if (cometIndex >= NUM_LEDS) {
    cometIndex = 0;
  }
}

void runMeteorRain() {
  fadeAllPixels(30);

  pixels.setPixelColor(meteorIndex, pixels.Color(currentR, currentG, currentB));
  pixels.show();

  meteorIndex++;

  if (meteorIndex >= NUM_LEDS) {
    meteorIndex = 0;
  }
}

void runFireEffect() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int cooling = random(0, 55);

    if (fireHeat[i] <= cooling) {
      fireHeat[i] = 0;
    } else {
      fireHeat[i] = fireHeat[i] - cooling;
    }
  }

  for (int i = NUM_LEDS - 1; i >= 2; i--) {
    fireHeat[i] = (fireHeat[i - 1] + fireHeat[i - 2] + fireHeat[i - 2]) / 3;
  }

  if (random(255) < 120) {
    int fireSize = NUM_LEDS;

    if (fireSize > 3) {
      fireSize = 3;
    }

    int y = random(fireSize);

    fireHeat[y] = fireHeat[y] + random(160, 255);

    if (fireHeat[y] > 255) {
      fireHeat[y] = 255;
    }
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    byte heat = fireHeat[i];

    byte r = heat;
    byte g = heat / 3;
    byte b = 0;

    if (heat > 180) {
      g = 100;
      b = heat - 180;
    }

    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }

  pixels.show();
}

void runTwinkle() {
  fadeAllPixels(40);

  int index = random(NUM_LEDS);

  uint8_t r = random(80, 255);
  uint8_t g = random(80, 255);
  uint8_t b = random(80, 255);

  pixels.setPixelColor(index, pixels.Color(r, g, b));
  pixels.show();
}

void runScanner() {
  pixels.clear();

  pixels.setPixelColor(scannerIndex, pixels.Color(currentR, currentG, currentB));
  pixels.show();

  scannerIndex += scannerDirection;

  if (scannerIndex >= NUM_LEDS - 1) {
    scannerIndex = NUM_LEDS - 1;
    scannerDirection = -1;
  }

  if (scannerIndex <= 0) {
    scannerIndex = 0;
    scannerDirection = 1;
  }
}

void runStrobe() {
  strobeState = !strobeState;

  if (strobeState) {
    showCurrentColor();
  } else {
    showOff();
  }
}

void runRandomColor() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(random(255), random(255), random(255)));
  }

  pixels.show();
}

void runRgbFade() {
  if (fadeStage == 0) {
    fadeG += 5;

    if (fadeG >= 250) {
      fadeG = 255;
      fadeStage = 1;
    }
  } else if (fadeStage == 1) {
    fadeR -= 5;

    if (fadeR <= 5) {
      fadeR = 0;
      fadeStage = 2;
    }
  } else if (fadeStage == 2) {
    fadeB += 5;

    if (fadeB >= 250) {
      fadeB = 255;
      fadeStage = 3;
    }
  } else if (fadeStage == 3) {
    fadeG -= 5;

    if (fadeG <= 5) {
      fadeG = 0;
      fadeStage = 4;
    }
  } else if (fadeStage == 4) {
    fadeR += 5;

    if (fadeR >= 250) {
      fadeR = 255;
      fadeStage = 5;
    }
  } else if (fadeStage == 5) {
    fadeB -= 5;

    if (fadeB <= 5) {
      fadeB = 0;
      fadeStage = 0;
    }
  }

  showColor(fadeR, fadeG, fadeB);
}

void runRainbowCycle() {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint16_t pixelHue = rainbowHue + (i * 65536L / NUM_LEDS);
    pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  }

  pixels.show();

  rainbowHue += 2048;
}

void runColorBounce() {
  pixels.clear();

  pixels.setPixelColor(bounceIndex, pixels.Color(currentR, currentG, currentB));
  pixels.show();

  bounceIndex += bounceDirection;

  if (bounceIndex >= NUM_LEDS - 1) {
    bounceIndex = NUM_LEDS - 1;
    bounceDirection = -1;
  }

  if (bounceIndex <= 0) {
    bounceIndex = 0;
    bounceDirection = 1;
  }
}

void runSnake() {
  pixels.clear();

  for (int i = 0; i < 3; i++) {
    int index = snakeHead - i;

    if (index < 0) {
      index = index + NUM_LEDS;
    }

    uint8_t level = 255 - (i * 80);

    pixels.setPixelColor(index, pixels.Color(
      (currentR * level) / 255,
      (currentG * level) / 255,
      (currentB * level) / 255
    ));
  }

  pixels.show();

  snakeHead++;

  if (snakeHead >= NUM_LEDS) {
    snakeHead = 0;
  }
}


// ---------- LOOP HANDLERS ----------

void handleLedAnimation() {
   if (currentMode == MODE_OFF) {
    pixels.clear();
    pixels.show();
    return;
  }

  if (currentMode == MODE_SOLID) {
    return;
  }
  if (millis() - lastEffectTime < effectSpeed) {
    return;
  }

  lastEffectTime = millis();

  if (currentMode == MODE_BLINK) {
    runBlink();
  } else if (currentMode == MODE_RAINBOW) {
    showRainbowFrame();
  } else if (currentMode == MODE_RAINBOW_BLINK) {
    runRainbowBlink();
  } else if (currentMode == MODE_BREATHING) {
    runBreathing();
  } else if (currentMode == MODE_COLOR_WIPE) {
    runColorWipe();
  } else if (currentMode == MODE_THEATER) {
    runTheaterChase();
  } else if (currentMode == MODE_SPARKLE) {
    runSparkle();
  } else if (currentMode == MODE_POLICE) {
    runPolice();
  } else if (currentMode == MODE_RUNNING_LIGHT) {
    runRunningLight();
  } else if (currentMode == MODE_COMET) {
    runComet();
  } else if (currentMode == MODE_METEOR_RAIN) {
    runMeteorRain();
  } else if (currentMode == MODE_FIRE_EFFECT) {
    runFireEffect();
  } else if (currentMode == MODE_TWINKLE) {
    runTwinkle();
  } else if (currentMode == MODE_SCANNER) {
    runScanner();
  } else if (currentMode == MODE_STROBE) {
    runStrobe();
  } else if (currentMode == MODE_RANDOM_COLOR) {
    runRandomColor();
  } else if (currentMode == MODE_RGB_FADE) {
    runRgbFade();
  } else if (currentMode == MODE_RAINBOW_CYCLE) {
    runRainbowCycle();
  } else if (currentMode == MODE_COLOR_BOUNCE) {
    runColorBounce();
  } else if (currentMode == MODE_SNAKE) {
    runSnake();
  }
}

void handleLedButton() {
  static bool lastStableState = HIGH;
  static bool lastReading = HIGH;
  static unsigned long lastDebounceTime = 0;
  static unsigned long buttonPressStart = 0;
  static bool longPressDone = false;

  const unsigned long debounceDelay = 50;
  const unsigned long longPressTime = 10000;   // 10 seconds

  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != lastStableState) {
      lastStableState = reading;

      if (lastStableState == LOW) {
        buttonPressStart = millis();
        longPressDone = false;

        Serial.println("Button pressed");
        Serial.println("Hold for 10 seconds for factory reset");
      }

      if (lastStableState == HIGH) {
        unsigned long pressDuration = millis() - buttonPressStart;

        if (!longPressDone && pressDuration < longPressTime) {
          buttonMode++;

          if (buttonMode > MODE_SNAKE) {
            buttonMode = MODE_OFF;
          }

          Serial.print("Short press detected. Mode: ");
          Serial.println(buttonMode);

          if (buttonMode == MODE_OFF) {
            ledOff();
          } else if (buttonMode == MODE_SOLID) {
            ledSolid();
          } else if (buttonMode == MODE_BLINK) {
            ledBlink();
          } else if (buttonMode == MODE_RAINBOW) {
            ledRainbow();
          } else if (buttonMode == MODE_BREATHING) {
            ledBreathing();
          } else if (buttonMode == MODE_COLOR_WIPE) {
            ledColorWipe();
          } else if (buttonMode == MODE_THEATER) {
            ledTheaterChase();
          } else if (buttonMode == MODE_SPARKLE) {
            ledSparkle();
          } else if (buttonMode == MODE_POLICE) {
            ledPolice();
          } else if (buttonMode == MODE_RUNNING_LIGHT) {
            ledRunningLight();
          } else if (buttonMode == MODE_COMET) {
            ledComet();
          } else if (buttonMode == MODE_METEOR_RAIN) {
            ledMeteorRain();
          } else if (buttonMode == MODE_FIRE_EFFECT) {
            ledFireEffect();
          } else if (buttonMode == MODE_TWINKLE) {
            ledTwinkle();
          } else if (buttonMode == MODE_SCANNER) {
            ledScanner();
          } else if (buttonMode == MODE_STROBE) {
            ledStrobe();
          } else if (buttonMode == MODE_RANDOM_COLOR) {
            ledRandomColor();
          } else if (buttonMode == MODE_RGB_FADE) {
            ledRgbFade();
          } else if (buttonMode == MODE_RAINBOW_CYCLE) {
            ledRainbowCycle();
          } else if (buttonMode == MODE_COLOR_BOUNCE) {
            ledColorBounce();
          } else if (buttonMode == MODE_SNAKE) {
            ledSnake();
          }
        }

        Serial.println("Button released");
      }
    }

    if (lastStableState == LOW && !longPressDone) {
      unsigned long pressDuration = millis() - buttonPressStart;

      if (pressDuration >= longPressTime) {
        longPressDone = true;

        Serial.println("LONG PRESS 10 SECONDS DETECTED");
        Serial.println("Factory reset will start now");

        ledStatusFactoryReset();

        factoryResetConfig();
      }
    }
  }

  lastReading = reading;
}
void ledStatusFactoryReset() {
  Serial.println("Factory reset LED pattern");

  for (int t = 0; t < 6; t++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    }
    pixels.show();
    delay(150);

    pixels.clear();
    pixels.show();
    delay(150);

    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    }
    pixels.show();
    delay(150);

    pixels.clear();
    pixels.show();
    delay(150);
  }
}