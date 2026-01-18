#pragma once

#include <Arduino.h>
#include <IPAddress.h>

/**
 * @file ui_state.h
 * @brief Gestion de l'état global de l'interface utilisateur.
 * @note Utiliser des structures statiques et éviter les objets String dynamiques pour optimiser la mémoire.
 */

namespace UiState {

/**
 * Structure containing all UI state information.
 */
struct State {
  bool wifiConnected = false;
  String currentSsid;
  IPAddress currentIp;
  bool mdnsOk = false;
  unsigned long lastUiRefresh = 0;
  bool rebootPrompt = false;
  unsigned long rebootHoldStart = 0;
};

/**
 * Get the global UI state instance.
 * @return Reference to global UI state
 */
State& getState();

/**
 * Reset UI state to initial values.
 */
void reset();

extern State g_uiState;

}  // namespace UiState
