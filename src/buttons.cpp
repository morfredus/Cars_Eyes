/**
 * @file buttons.cpp
 * @brief Implémentation de la gestion des boutons physiques.
 * @note Utiliser des variables locales et statiques pour limiter l'utilisation mémoire. Éviter les allocations dynamiques.
 */

#include "buttons.h"
#include "board_config.h"
#include "config.h"
#include "tft_display.h"
#include "neopixel.h"
#include "ui_state.h"
#include <Arduino.h>

namespace Buttons {

static uint32_t btn1LastChange = 0;
static uint32_t btn2LastChange = 0;
static bool btn1State = true; // High (pullup)
static bool btn2State = true;
static bool warningActive = false;
static bool signalLong1 = false;
static bool signalLong2 = false;

/**
 * @brief Initialise les broches des boutons en entrée avec pull-up.
 */
void init() {
  pinMode(BUTTON_BOOT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
}

/**
 * @brief Gère la logique des boutons et du reboot.
 * À appeler régulièrement dans la boucle principale.
 * Utilise des variables statiques pour le suivi d'état.
 */
void handle() {
  UiState::State& ui = UiState::getState();
  const uint32_t now = millis();
  
  // Read Raw
  bool b1 = digitalRead(BUTTON_1_PIN);
  bool b2 = digitalRead(BUTTON_2_PIN);
  bool bBoot = digitalRead(BUTTON_BOOT_PIN) == LOW;

  // --- Turn Signal Logic ---
  
    // --- Turn Signal Logic ---
  static bool lastB1 = HIGH;
  static bool lastB2 = HIGH;
  static uint32_t pressStart1 = 0;
  static uint32_t pressStart2 = 0;
  static bool handled1 = false;
  static bool handled2 = false;
  static bool hazardActive = false;

  // 1. Hazard Detection (Both Pressed)
  // If both buttons are down, trigger hazard immediately
  if (b1 == LOW && b2 == LOW) {
      if (!hazardActive) {
          NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::HAZARD, true); // Continuous
          hazardActive = true;
          handled1 = true; // Block individual triggers
          handled2 = true;
      }
  } 
  else if (b1 == HIGH && b2 == HIGH) {
      hazardActive = false; // Reset when both released
  }

  // 2. Button 1 (Left)
  if (b1 != lastB1) { // Edge
      if (b1 == LOW) { // Press
          pressStart1 = now;
          handled1 = false;
      } else { // Release
          if (!handled1 && !hazardActive) {
              // Short press release
              NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_LEFT, false);
          }
      }
      lastB1 = b1;
  }
  // B1 Long Press
  if (b1 == LOW && !handled1 && !hazardActive) {
      if (now - pressStart1 > 500) {
           NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_LEFT, true);
           handled1 = true;
      }
  }

  // 3. Button 2 (Right)
  if (b2 != lastB2) { // Edge
      if (b2 == LOW) { // Press
          pressStart2 = now;
          handled2 = false;
      } else { // Release
          if (!handled2 && !hazardActive) {
              // Short press release
              NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_RIGHT, false);
          }
      }
      lastB2 = b2;
  }
  // B2 Long Press
  if (b2 == LOW && !handled2 && !hazardActive) {
      if (now - pressStart2 > 500) {
           NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_RIGHT, true);
           handled2 = true;
      }
  }



  // --- Boot Button Logic (Existing) ---
  const bool bootPressed = bBoot; 
  
  if (bootPressed && !ui.rebootPrompt) {
    ui.rebootPrompt = true;
    ui.rebootHoldStart = millis();
    TftDisplay::drawRebootPrompt(0);
  }

  if (ui.rebootPrompt) {
    const uint32_t elapsed = millis() - ui.rebootHoldStart;
    const uint32_t requiredMs = static_cast<uint32_t>(kSystemConfig.rebootHoldSeconds) * 1000UL;
    uint8_t percent = elapsed >= requiredMs ? 100 : static_cast<uint8_t>((elapsed * 100) / requiredMs);
    TftDisplay::drawRebootPrompt(percent);
    
    if (!bootPressed) {
      ui.rebootPrompt = false;
      TftDisplay::updateMainScreen(true);
      return;
    }
    
    if (percent >= 100) {
      #if defined(ENV_ESP32S3_N16R8)
      NeoPixel::setStatusLed(NeoPixel::makeColor(0, 0, 255));  // Blue LED
      #endif
      ESP.restart();
    }
  }
}

}  // namespace Buttons
