/**
 * @file tft_display.cpp
 * @brief Implémentation de la gestion de l'affichage TFT et du rétroéclairage.
 * @note Utiliser des buffers statiques pour le rendu graphique et éviter les allocations dynamiques pour optimiser la mémoire.
 */

#include "tft_display.h"
#include "config.h"
#include "board_config.h"
#include "ui_state.h"
#include "network.h"
#include "neopixel.h"
#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>

// Global TFT object - initialized in main.cpp
Adafruit_ST7789 tft(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN);

namespace TftDisplay {
/**
 * @brief Initialise le rétroéclairage avec contrôle PWM ou digital IO.
 */
void initBacklight() {
  if (DISPLAY_BL_PIN == 255) {
    return;
  }
  // Simplified Backlight Control for reliable startup
  // Instead of PWM (ledc), we use simple digital IO to ensure the screen turns on.
  pinMode(DISPLAY_BL_PIN, OUTPUT);
  if (kSystemConfig.backlightLevel > 0) {
    digitalWrite(DISPLAY_BL_PIN, HIGH);
  } else {
    digitalWrite(DISPLAY_BL_PIN, LOW);
  }
}

/**
 * @brief Initialise l'afficheur SPI et ST7789.
 */
void setupDisplay() {
  Serial.println("[DISPLAY] Starting SPI with low frequency...");
  // Use low frequency at startup (20 MHz instead of 27 MHz)
  SPI.begin(DISPLAY_SCK_PIN, DISPLAY_MISO_PIN, DISPLAY_MOSI_PIN);
  
  // Increase delay and ensure SPI stabilizes
  delay(50);
  
  Serial.println("[DISPLAY] Initializing ST7789 1.54\"...");
  tft.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  delay(100);
  
  Serial.println("[DISPLAY] Setting rotation...");
  tft.setRotation(kSystemConfig.lcdRotation);
  
  // Invert Colors if needed (Often required for ST7789)
  // tft.invertDisplay(true); 
  
  // Clear screen to black first
  tft.fillScreen(ST77XX_BLACK);
  delay(50);
  
  Serial.println("[DISPLAY] Initializing backlight...");
  initBacklight();
  
  // Simple test with delay
  Serial.println("[DISPLAY] Test: filling RED...");
  tft.fillScreen(ST77XX_RED);
  delay(500);
  
  Serial.println("[DISPLAY] Drawing boot screen...");
  drawBootScreen("Boot", 5);
  Serial.println("[DISPLAY] Setup complete!");
}

uint16_t printWrap(const String& text, uint8_t textSize, uint16_t x, uint16_t yStart, uint16_t maxWidthPx, uint8_t maxLines) {
  const uint16_t charWidth = 6 * textSize;
  const uint16_t charHeight = 8 * textSize;
  const uint16_t maxChars = maxWidthPx / charWidth;
  if (maxChars == 0 || maxLines == 0) return yStart;

  String remaining = text;
  uint16_t y = yStart;
  for (uint8_t line = 0; line < maxLines && remaining.length() > 0; ++line) {
    String slice;
    if (remaining.length() <= maxChars) {
      slice = remaining;
      remaining = "";
    } else {
      slice = remaining.substring(0, maxChars);
      remaining = remaining.substring(maxChars);
      // If this is the last allowed line and there is still text, add ellipsis
      if (line + 1 == maxLines) {
        if (slice.length() >= 3) {
          slice = slice.substring(0, slice.length() - 3) + "...";
        }
        remaining = "";
      }
    }
    tft.setCursor(x, y);
    tft.print(slice);
    y += charHeight;
  }
  return y;
}

String fitText(const String& text, uint8_t textSize, uint16_t maxWidthPx) {
  const uint16_t charWidth = 6 * textSize;  // default Adafruit_GFX font width
  const uint16_t maxChars = maxWidthPx / charWidth;
  if (maxChars == 0) return "";
  if (text.length() <= maxChars) return text;
  if (maxChars <= 3) return text.substring(0, maxChars);  // not enough room for ellipsis
  return text.substring(0, maxChars - 3) + "...";  // ASCII ellipsis to avoid unsupported glyphs
}

void printFit(uint16_t x, uint16_t y, const String& text, uint8_t textSize, uint16_t maxWidthPx) {
  tft.setTextSize(textSize);
  tft.setCursor(x, y);
  tft.print(fitText(text, textSize, maxWidthPx));
}

// Colors for Cars theme
constexpr uint16_t CARS_RED = 0xC800;     // Deeper red
constexpr uint16_t CARS_YELLOW = 0xFFE0;  // Standard bright yellow
constexpr uint16_t CARS_WHITE = 0xFFFF;
constexpr uint16_t CARS_BLACK = 0x0000;

// Helper to draw lightning
void drawLightning(int x, int y, int size, uint16_t color) {
  // Simple lightning shape using triangles
  tft.fillTriangle(x + size/2, y, x + size, y, x, y + size, color);
  tft.fillTriangle(x, y + size, x + size/2, y + size/2, x - size/4, y + size*1.5, color);
}

void drawBootScreen(const char* message, uint8_t percent) {
  // Only clear screen if it's the start (percent <= 5) to set background
  if (percent <= 5) { 
    tft.fillScreen(CARS_RED);
    
    // Draw Logo
    int cx = tft.width() / 2;
    int cy = tft.height() / 3; // Position in upper third
    
    tft.setTextColor(CARS_YELLOW);
    tft.setTextSize(3);
    // Centered "CARS" (roughly 18px per char width at size 3)
    tft.setCursor(cx - (4 * 18 / 2), cy);
    tft.print("CARS");

    tft.setTextColor(CARS_WHITE);
    tft.setTextSize(2);
    // Centered "EYES" (roughly 12px per char width at size 2)
    tft.setCursor(cx - (4 * 12 / 2), cy + 40);
    tft.print("EYES");
    
    // Lightning Bolt
    drawLightning(cx + 40, cy - 20, 20, CARS_YELLOW);
  }

  // Draw Status Message
  int barY = tft.height() - 50;
  // Clear previous message area
  tft.fillRect(0, barY - 40, tft.width(), 30, CARS_RED);
  
  tft.setTextColor(CARS_WHITE);
  tft.setTextSize(2);
  int msgLen = strlen(message);
  // Estimate width: 12px per char (size 2)
  int msgW = msgLen * 12; 
  tft.setCursor((tft.width() - msgW)/2, barY - 30);
  tft.print(message);

  if (kSystemConfig.enableBootBar) {
    int barW = tft.width() - 40;
    int barX = 20; 
    
    // Draw Bar Outline
    tft.drawRect(barX, barY, barW, 20, CARS_WHITE);
    
    // Fill Bar
    int fillW = (percent * (barW - 4)) / 100;
    tft.fillRect(barX + 2, barY + 2, fillW > 0 ? fillW : 0, 16, CARS_YELLOW);
  }
}

void updateMainScreen(bool force) {
  const bool wifiNow = WiFi.isConnected();
  const String ssidNow = wifiNow ? WiFi.SSID() : "Offline";
  const IPAddress ipNow = wifiNow ? WiFi.localIP() : IPAddress();
  
  // Access UI state
  UiState::State& ui = UiState::getState();

  // Access Eye state for mode check
  #if defined(ENV_ESP32S3_N16R8)
  const NeoPixel::EyeState& eyeState = NeoPixel::getState();
  const NeoPixel::AnimationType currentAnim = eyeState.currentAnimation;
  const bool autoPlay = eyeState.autoPlay;
  #else
  const int currentAnim = 0;
  const bool autoPlay = false;
  #endif

  // Track previous animation state to trigger updates
  static int lastAnimId = -1;
  static bool lastAutoPlay = false;
  
  if (!force && 
      wifiNow == ui.wifiConnected && 
      ssidNow == ui.currentSsid && 
      ipNow == ui.currentIp && 
      (int)currentAnim == lastAnimId && 
      autoPlay == lastAutoPlay) {
    return;
  }
  
  ui.wifiConnected = wifiNow;
  ui.currentSsid = ssidNow;
  ui.currentIp = ipNow;
  lastAnimId = (int)currentAnim;
  lastAutoPlay = autoPlay;

  tft.fillScreen(CARS_BLACK);
  
  // 1. Checkerboard Header (Top 20px)
  int w = tft.width();
  int sqSize = 10;
  for (int y = 0; y < 20; y += sqSize) {
    for (int x = 0; x < w; x += sqSize) {
      bool black = ((x/sqSize) + (y/sqSize)) % 2 == 0;
      tft.fillRect(x, y, sqSize, sqSize, black ? CARS_BLACK : CARS_WHITE);
    }
  }

  // 2. Red Banner (agrandie pour la version)
  tft.fillRect(0, 20, w, 45, CARS_RED);
  tft.setTextColor(CARS_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 27);
  tft.print("CARS EYES");
  
  // Version sous le titre
  tft.setTextSize(1);
  tft.setTextColor(CARS_YELLOW);
  String versionStr = "v";
  versionStr += projectVersion();
  tft.setCursor(10, 45);
  tft.print(versionStr);
  
  // 3. Info Content (décalé vers le bas)
  int y = 75;
  tft.setTextColor(CARS_WHITE); 
  tft.setTextSize(2);
  
  // WiFi
  tft.setCursor(10, y);
  tft.print("WiFi: ");
  if (wifiNow) {
    tft.setTextColor(0x07E0); // Green
    tft.print("ON");
  } else {
    tft.setTextColor(CARS_RED);
    tft.print("OFF");
  }
  
  y += 25;
  if (wifiNow) {
     tft.setTextSize(1);
     tft.setTextColor(CARS_YELLOW);
     tft.setCursor(10, y);
     tft.printf("SSID: %s", ssidNow.c_str());
     y += 15;
     tft.setCursor(10, y);
     tft.printf("IP: %s", ipNow.toString().c_str());
     y += 15;
     tft.setCursor(10, y);
     tft.printf("mDNS: %s.local", kSystemConfig.mdnsHost);
     y += 20;  // Espace après mDNS
  }
  
  // Eyes Status (Bottom)
  // Ligne de séparation rouge après mDNS
  tft.drawLine(0, y, w, y, CARS_RED);
  y += 10;
  
  #if defined(ENV_ESP32S3_N16R8)
  // eyeState is already declared at the top of the function
  const char* animNames[] = {"IDLE", "BLINK", "WINK_L", "WINK_R", "LOOK_L", "LOOK_R", 
                              "LOOK_U", "LOOK_D", "HAPPY", "SAD", "ANGRY", "SURPRISED", "SLEEP",
                              "TURN_L", "TURN_R", "HAZARD", "CUSTOM", "OFF"};
  const int animIndex = static_cast<int>(eyeState.currentAnimation);
  const char* animName = (animIndex >= 0 && animIndex < 18) ? animNames[animIndex] : "UNKNOWN";
  
  tft.setTextSize(2);
  tft.setTextColor(CARS_WHITE);
  tft.setCursor(10, y);
  tft.print("MODE:");
  tft.setTextColor(CARS_YELLOW);
  tft.setCursor(70, y);
  tft.print(animName);
  
  if (eyeState.autoPlay) {
      tft.setTextColor(0x07E0);
      tft.setTextSize(1);
      tft.setCursor(w - 40, y + 5);
      tft.print("AUTO");
  }
  #endif
  
  // Decorative Lightning
  drawLightning(w - 30, tft.height() - 40, 20, CARS_YELLOW);
}

void drawRebootPrompt(uint8_t percent) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print("Reboot ?");
  uint16_t barWidth = DISPLAY_WIDTH - (2 * kBootBarMargin);
  uint16_t filled = (percent * barWidth) / 100;
  uint16_t y = DISPLAY_HEIGHT / 2;
  tft.drawRect(kBootBarMargin, y, barWidth, 20, ST77XX_WHITE);
  tft.fillRect(kBootBarMargin + 2, y + 2, filled > 4 ? filled - 4 : 0, 16, ST77XX_ORANGE);
}

}  // namespace TftDisplay
