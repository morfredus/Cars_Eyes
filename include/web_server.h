#pragma once

/**
 * @file web_server.h
 * @brief Initialisation et gestion du serveur web HTTP.
 * @note Utiliser des buffers statiques pour les réponses HTTP et limiter l'utilisation de String.
 */

#include <WebServer.h>

namespace HttpServer {

/**
 * Initialize and setup the web server with all routes.
 * Configures endpoints for dashboard, OTA, and status API.
 * Only initializes if kSystemConfig.enableWebUi is true.
 */
void init();

/**
 * Handle incoming HTTP requests.
 * Should be called regularly in the main loop.
 */
void handleClient();

}  // namespace HttpServer
