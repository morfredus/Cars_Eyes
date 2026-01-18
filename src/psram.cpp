/**
 * @file psram.cpp
 * @brief Implémentation de l'initialisation et gestion de la PSRAM.
 * @note Vérifier la disponibilité de la PSRAM avant usage et éviter les allocations dynamiques inutiles.
 */

#include "psram.h"
#include <Arduino.h>

#if defined(ENV_ESP32S3_N16R8)
#include <esp32-hal-psram.h>
#endif

namespace Psram {
/**
 * @brief Initialise la PSRAM si disponible et affiche le statut.
 */
void init() {
#if defined(ENV_ESP32S3_N16R8)
  if (!psramInit()) {
    Serial.println("[PSRAM] Init failed or PSRAM not detected.");
    return;
  }
  const size_t total = ESP.getPsramSize();
  const size_t free = ESP.getFreePsram();
  Serial.printf("[PSRAM] Enabled: %u / %u bytes free.\n", static_cast<unsigned>(free), static_cast<unsigned>(total));
#else
  Serial.println("[PSRAM] Not available on this board.");
#endif
}
}  // namespace Psram
