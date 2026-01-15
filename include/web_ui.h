#pragma once

#include <Arduino.h>

namespace WebUI {

/**
 * Build the main dashboard page HTML with eye animation controls.
 * Displays system info, WiFi status, and eye control interface.
 * @return HTML string for dashboard page
 */
String buildRootPage();

/**
 * Build the OTA update page HTML.
 * Allows file upload for firmware update with progress tracking.
 * @return HTML string for OTA page
 */
String buildOtaPage();

/**
 * Build the custom pixel editor page HTML.
 * Allows pixel-by-pixel customization with color selection.
 * @return HTML string for custom editor page
 */
String buildCustomEditorPage();

/**
 * Build the system status JSON response.
 * Contains project info, hardware details, network status, and eye state.
 * @return JSON string with system status
 */
String buildStatusJson();

/**
 * Build the eye animation control JSON response.
 * Contains current animation state, brightness, colors, etc.
 * @return JSON string with eye state
 */
String buildEyeStatusJson();

}  // namespace WebUI
