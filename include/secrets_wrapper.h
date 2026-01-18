#ifndef SECRETS_WRAPPER_H
#define SECRETS_WRAPPER_H

#include <stddef.h>

/**
 * @file secrets_wrapper.h
 * @brief Interface d'accès sécurisée aux identifiants sensibles.
 * @note Utiliser des pointeurs constants et éviter la copie de chaînes pour optimiser la mémoire.
 */

// ============================================================================
// SECRETS WRAPPER - Interface d'accès sécurisée aux credentials
// ============================================================================
// N'inclut PAS secrets.h pour éviter les conflits de définitions multiples.
// Utilise ces fonctions pour accéder aux données de secrets.h
// ============================================================================

// Récupère le SSID du réseau WiFi à l'index donné
const char* getWifiSsid(size_t index);

// Récupère le mot de passe du réseau WiFi à l'index donné
const char* getWifiPassword(size_t index);

// Retourne le nombre total de réseaux WiFi configurés
size_t getWifiNetworksCount();

#endif // SECRETS_WRAPPER_H
