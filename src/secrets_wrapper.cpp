// ============================================================================
// SECRETS WRAPPER
// ============================================================================
// Ce fichier est le SEUL qui inclut secrets.h pour éviter les conflits
// de définitions multiples lors du linking.
// Les autres fichiers doivent utiliser les fonctions ci-dessous.
// ============================================================================

#include <cstddef>
#include "secrets.h"

/**
 * @file secrets_wrapper.cpp
 * @brief Implémentation de l'accès sécurisé aux identifiants sensibles.
 * @note Utiliser des pointeurs constants et éviter la copie de chaînes pour optimiser la mémoire.
 */

/**
 * @brief Retourne le SSID du réseau WiFi à l'index donné.
 */
const char* getWifiSsid(size_t index) {
  constexpr size_t wifiCount = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
  if (index >= wifiCount) return nullptr;
  return WIFI_NETWORKS[index][0];
}

/**
 * @brief Retourne le mot de passe du réseau WiFi à l'index donné.
 */
const char* getWifiPassword(size_t index) {
  constexpr size_t wifiCount = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
  if (index >= wifiCount) return nullptr;
  return WIFI_NETWORKS[index][1];
}

/**
 * @brief Retourne le nombre total de réseaux WiFi configurés.
 */
size_t getWifiNetworksCount() {
  return sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
}
