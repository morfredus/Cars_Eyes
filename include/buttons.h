#pragma once
/**
 * @file buttons.h
 * @brief Gestion des boutons physiques.
 * @note Pour optimiser la mémoire, utiliser des variables locales et éviter les buffers dynamiques.
 */

namespace Buttons {

/**
 * @brief Initialise tous les boutons en entrée avec pull-up.
 */
void init();

/**
 * @brief Gère la logique des boutons et du reboot.
 * À appeler régulièrement dans la boucle principale.
 */
void handle();

}  // namespace Buttons
