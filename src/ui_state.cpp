/**
 * @file ui_state.cpp
 * @brief Implémentation de la gestion de l'état global de l'interface utilisateur.
 * @note Utiliser des structures statiques et éviter les objets String dynamiques pour optimiser la mémoire.
 */

#include "ui_state.h"

namespace UiState {
/**
 * @brief Retourne l'instance globale de l'état UI.
 */
State& getState() {
  return g_uiState;
}

/**
 * @brief Réinitialise l'état UI aux valeurs initiales.
 */
void reset() {
  g_uiState = State();
}
}  // namespace UiState

namespace UiState {
State g_uiState;
}
