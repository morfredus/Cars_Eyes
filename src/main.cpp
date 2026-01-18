#include <Arduino.h>
#include <ArduinoOTA.h>

#include "board_config.h"
#include "config.h"

// Modular components
#include "psram.h"
#include "tft_display.h"
#include "buttons.h"
#include "neopixel.h"
#include "network.h"
#include "web_server.h"
#include "ui_state.h"


/**
 * @file main.cpp
 * @brief Point d'entrée principal du projet Cars Eyes.
 * @note Initialiser les modules dans l'ordre pour optimiser la mémoire et éviter les allocations inutiles.
 */

/**
 * @brief Fonction d'initialisation Arduino.
 * Initialise tous les modules et affiche l'état système.
 */
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n[MAIN] Starting...");
  
  // Initialize basic components
  Psram::init();
  Buttons::init();
  TftDisplay::setupDisplay();
  NeoPixel::init();

  // Show WiFi connection progress
  TftDisplay::drawBootScreen("WiFi", 15);
  const bool wifiOk = Network::connectWifiWithFeedback();
  UiState::getState().wifiConnected = wifiOk;
  
  // Initialize network services based on WiFi status
  if (wifiOk) {
    TftDisplay::drawBootScreen("WiFi OK", 70);
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::setStatusLed(NeoPixel::makeColor(0, 80, 0));  // Green
    #endif

    TftDisplay::drawBootScreen("Starting mDNS", 75);
    UiState::getState().mdnsOk = Network::initMdns();
    
    TftDisplay::drawBootScreen("Starting OTA", 80);
    Network::initOta();
    
    TftDisplay::drawBootScreen("Starting web", 85);
    HttpServer::init();
  } else {
    TftDisplay::drawBootScreen("WiFi fail", 70);
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::setStatusLed(NeoPixel::makeColor(80, 0, 0));  // Red
    #endif
    
    UiState::getState().mdnsOk = false;
    Serial.println("[MAIN] Network services disabled (WiFi connection failed)");
  }

  // Show ready screen
  TftDisplay::drawBootScreen("Ready", 100);
  delay(500);
  TftDisplay::updateMainScreen(true);
  
  Serial.println("[MAIN] Setup complete!");
}

/**
 * @brief Boucle principale Arduino.
 * Gère les tâches périodiques et le serveur web.
 */
void loop() {
  // Handle network and web requests
  HttpServer::handleClient();
  ArduinoOTA.handle();
  
  // Handle user input
  Buttons::handle();

  // Update NeoPixel animations
  NeoPixel::update();

  // Update UI periodically
  const unsigned long now = millis();
  if (now - UiState::getState().lastUiRefresh > 1000) {
    UiState::getState().lastUiRefresh = now;
    TftDisplay::updateMainScreen();
    
    // Update status LED
    #if defined(ENV_ESP32S3_N16R8)
    const uint32_t color = Network::isConnected() 
      ? NeoPixel::makeColor(0, 30, 0)    // Green
      : NeoPixel::makeColor(30, 0, 0);   // Red
    NeoPixel::setStatusLed(color);
    #endif
  }
}