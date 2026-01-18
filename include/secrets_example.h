#pragma once

/**
 * @file secrets_example.h
 * @brief Exemple de stockage des identifiants WiFi.
 * @note Utiliser des tableaux statiques pour limiter l'utilisation mémoire et garder secrets.h hors du contrôle de version.
 */

// Duplicate this file as secrets.h and list your WiFi networks here.
// Keep secrets.h out of version control.
static const char* WIFI_NETWORKS[][2] = {
    {"MyHomeSSID", "SuperSecretPass"},
    {"MyPhoneHotspot", "AnotherSecret"}
};
