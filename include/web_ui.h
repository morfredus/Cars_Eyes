#pragma once

#include <Arduino.h>

namespace WebUI {

/**
 * @brief Génère la page principale du tableau de bord avec les contrôles d'animation des yeux.
 * Affiche les infos système, le statut WiFi et l'interface de contrôle des yeux.
 *
 * @note Pour optimiser la mémoire, la chaîne HTML est construite dans un buffer réservé (voir implémentation).
 *
 * @return Chaîne HTML (String) pour la page dashboard. Préférer l'utilisation d'un buffer statique si la mémoire RAM est critique.
 */
String buildRootPage();

/**
 * @brief Génère la page HTML de mise à jour OTA.
 * Permet l'upload de firmware avec suivi de progression.
 *
 * @note Optimiser la mémoire en réservant la taille du buffer HTML à l'avance.
 *
 * @return Chaîne HTML (String) pour la page OTA. Utiliser un buffer statique si possible.
 */
String buildOtaPage();

/**
 * @brief Génère la page HTML de l'éditeur de pixels personnalisés.
 * Permet la personnalisation pixel par pixel avec sélection de couleur.
 *
 * @note Pour les pages volumineuses, privilégier la construction dans un buffer statique ou réservé.
 *
 * @return Chaîne HTML (String) pour l'éditeur personnalisé.
 */
String buildCustomEditorPage();

/**
 * @brief Génère la réponse JSON du statut système.
 * Contient les infos projet, matériel, réseau et état des yeux.
 *
 * @note Pour limiter l'utilisation mémoire, construire le JSON dans un buffer local ou utiliser ArduinoJson avec un StaticJsonDocument.
 *
 * @return Chaîne JSON (String) avec le statut système.
 */
String buildStatusJson();

/**
 * @brief Génère la réponse JSON pour le contrôle d'animation des yeux.
 * Contient l'état courant de l'animation, la luminosité, les couleurs, etc.
 *
 * @note Utiliser un StaticJsonDocument pour optimiser la mémoire lors de la construction du JSON.
 *
 * @return Chaîne JSON (String) avec l'état des yeux.
 */
String buildEyeStatusJson();

}  // namespace WebUI
