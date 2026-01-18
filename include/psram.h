#pragma once

/**
 * @file psram.h
 * @brief Initialisation et gestion de la PSRAM sur la carte.
 * @note Pour optimiser la mémoire, vérifier la disponibilité de la PSRAM avant usage et éviter les allocations dynamiques inutiles.
 */

namespace Psram {

/**
 * @brief Initialise la PSRAM si disponible.
 * Affiche le statut et la mémoire libre sur le port série.
 * Fonctionne uniquement sur les cartes compatibles PSRAM (ex: ESP32S3).
 */
void init();

}  // namespace Psram
