#pragma once

#include <Arduino.h>

/**
 * @file web_styles.h
 * @brief Gestion des styles CSS et scripts pour l'interface web.
 * @note Utiliser des chaînes constantes (const char*) pour les styles et scripts afin d'optimiser la mémoire.
 */

namespace WebStyles {

/**
 * Get the common CSS styles for dashboard and UI pages.
 * Includes layout, colors, responsive design, and component styling.
 * @return CSS string with all common styles
 */
const char* getDashboardStyles();

/**
 * Get the CSS styles specific for OTA upload page.
 * Includes form styling, progress bar, and upload UI elements.
 * @return CSS string for OTA styles
 */
const char* getOtaStyles();

/**
 * Get the JavaScript code for OTA upload functionality.
 * Handles file selection, upload progress, and error handling.
 * @return JavaScript string for OTA page functionality
 */
const char* getOtaScript();

}  // namespace WebStyles
