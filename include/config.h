/**
 * @file config.h
 * @brief Paramètres utilisateur centralisés pour l'application.
 * @note Utiliser des constantes et des structures statiques pour limiter l'utilisation mémoire.
 */

#pragma once
#include <Arduino.h>

// Centralized user-facing settings for the application.
struct SystemConfig {
    const char* mdnsHost; ///< Nom d'hôte mDNS
    uint8_t lcdRotation; ///< Rotation de l'écran LCD
    uint8_t backlightLevel; ///< Niveau de rétroéclairage (0-255)
    uint32_t wifiConnectTimeoutMs; ///< Timeout connexion WiFi
    uint32_t wifiRetryDelayMs; ///< Délai entre tentatives WiFi
    uint8_t maxWifiAttempts; ///< Nombre max de tentatives WiFi
    uint8_t rebootHoldSeconds; ///< Durée appui reboot
    bool enableBootBar; ///< Affichage barre de boot
    bool enableWebUi; ///< Activation Web UI
    bool enableNeoPixel; ///< Activation NeoPixel
};

/**
 * @brief Configuration système par défaut (constante).
 */
constexpr SystemConfig kSystemConfig{
    .mdnsHost = "cars-eyes",
    .lcdRotation = 2,               // Landscape by default
    .backlightLevel = 255,          // Full brightness
    .wifiConnectTimeoutMs = 12000,  // Per attempt
    .wifiRetryDelayMs = 500,        // Between WiFiMulti polls
    .maxWifiAttempts = 10,
    .rebootHoldSeconds = 3,
    .enableBootBar = true,
    .enableWebUi = true,
    .enableNeoPixel = true
};

/**
 * @brief Retourne le nom du projet.
 */
inline const char* projectName() { return PROJECT_NAME; }

/**
 * @brief Retourne la version du projet.
 */
inline const char* projectVersion() { return PROJECT_VERSION; }
