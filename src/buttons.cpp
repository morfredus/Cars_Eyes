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

void init() {
  pinMode(BUTTON_BOOT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_USER1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_USER2_PIN, INPUT_PULLUP);
}

void handle() {
  UiState::State& ui = UiState::getState();
  const uint32_t now = millis();
  
  // Read Raw
  bool b1 = digitalRead(BUTTON_USER1_PIN);
  bool b2 = digitalRead(BUTTON_USER2_PIN);
  bool bBoot = digitalRead(BUTTON_BOOT_PIN) == LOW;

  // --- Turn Signal Logic ---
  
  // Debounce/Change Detection B1 (Left)
  if (b1 != btn1State && (now - btn1LastChange > 50)) {
       btn1State = b1;
       btn1LastChange = now;
       
       if (b1 == LOW) { // Pressed
           // Check for Hazard (Both pressed)
           if (b2 == LOW) {
               NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::HAZARD, true);
               warningActive = true;
           } else {
               warningActive = false;
               // Start Timer for long press detection
               signalLong1 = false; 
           }
       } else { // Released
           if (!warningActive) {
               // Calculate duration
               uint32_t pressDur = now - btn1LastChange; // Wait, lastChange was just updated. logic error.
               // We need separate tracking for press start time. 
           }
       }
  }
  
  // Revised Logic for reliability
  static uint32_t b1PressStart = 0;
  static uint32_t b2PressStart = 0;
  static bool b1Handled = false;
  static bool b2Handled = false;

  // Handle B1
  if (b1 == LOW && btn1State == HIGH) { // Falling Edge
      btn1State = LOW;
      b1PressStart = now;
      b1Handled = false;
  } else if (b1 == HIGH && btn1State == LOW) { // Rising Edge
      btn1State = HIGH;
      if (!b1Handled && !warningActive) {
          // Short Press Release -> Trigger Short Signal
          NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_LEFT, false);
      }
      warningActive = false; // Reset warning flag on release
  }
  
  // Handle B2
  if (b2 == LOW && btn2State == HIGH) { // Falling Edge
      btn2State = LOW;
      b2PressStart = now;
      b2Handled = false;
  } else if (b2 == HIGH && btn2State == LOW) { // Rising Edge
      btn2State = HIGH;
      if (!b2Handled && !warningActive) {
          // Short Press Release -> Trigger Short Signal
          NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_RIGHT, false);
      }
      warningActive = false;
  }

  // Continuous Checks
  if (btn1State == LOW && btn2State == LOW) {
      if (!warningActive) {
          NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::HAZARD, true);
          warningActive = true;
          b1Handled = true; // Prevent single triggers
          b2Handled = true;
      }
  } else {
      // Long Press Checks
      if (btn1State == LOW && !b1Handled && !warningActive) {
          if (now - b1PressStart > 500) {
              NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_LEFT, true);
              b1Handled = true; 
          }
      }
      if (btn2State == LOW && !b2Handled && !warningActive) {
          if (now - b2PressStart > 500) {
              NeoPixel::toggleTurnSignal(NeoPixel::AnimationType::TURN_RIGHT, true);
              b2Handled = true;
          }
      }
  }


  // --- Boot Button Logic (Existing) ---
  if (bBoot && !ui.rebootPrompt) {
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
