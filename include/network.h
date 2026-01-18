#pragma once

#include <Arduino.h>

/**
 * @file network.h
 * @brief Gestion du réseau WiFi, mDNS et OTA.
 * @note Utiliser des buffers locaux pour les réponses réseau et éviter les allocations dynamiques.
 */

namespace Network {

/**
 * @brief Initialise la connexion WiFi multi-réseaux avec feedback.
 * @return true si connecté, false sinon
 */
bool connectWifiWithFeedback();

/**
 * @brief Initialise le service mDNS avec le nom d'hôte configuré.
 * @return true si mDNS initialisé
 */
bool initMdns();

/**
 * @brief Vérifie si le service mDNS est actif.
 * @return true si mDNS actif
 */
bool isMdnsRunning();

/**
 * @brief Initialise le service ArduinoOTA pour les mises à jour.
 */
void initOta();

/**
 * @brief Gère la résolution DNS multicast. À appeler périodiquement.
 */
void handleMdns();

/**
 * Get current WiFi connection status.
 * @return true if WiFi is connected, false otherwise
 */
bool isConnected();

/**
 * Get current WiFi SSID.
 * @return SSID string or "Offline" if not connected
 */
String getSsid();

/**
 * Get current WiFi IP address.
 * @return IP address as string
 */
String getIpAddress();

}  // namespace Network
