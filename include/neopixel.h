#pragma once

#include <Arduino.h>

/**
 * @file neopixel.h
 * @brief Contrôle des animations et couleurs NeoPixel.
 * @note Utiliser des buffers statiques pour les animations et limiter l'utilisation de String.
 */

namespace NeoPixel {

// ============================================================================
// TYPES ET STRUCTURES
// ============================================================================

/**
 * @brief Types d'animations des yeux.
 */
enum class AnimationType {
  IDLE = 0,        ///< Yeux ouverts, inactifs
  BLINK,           ///< Clignement rapide
  WINK_LEFT,       ///< Clin d'œil gauche
  WINK_RIGHT,      ///< Clin d'œil droit
  LOOK_LEFT,       ///< Regarde à gauche
  LOOK_RIGHT,      ///< Regarde à droite
  LOOK_UP,         ///< Regarde en haut
  LOOK_DOWN,       ///< Regarde en bas
  HAPPY,           ///< Expression joyeuse
  SAD,             ///< Expression triste
  ANGRY,           ///< Expression en colère
  SURPRISED,       ///< Expression surprise
  SLEEP,           ///< Yeux fermés (sommeil)
  TURN_LEFT,       ///< Flèche gauche (clignotant)
  TURN_RIGHT,      ///< Flèche droite (clignotant)
  HAZARD,          ///< Feux de détresse
  CUSTOM,          ///< Motif personnalisé via Web UI
  OFF              ///< LEDs éteintes
};

/**
 * @brief Préréglages de schémas de couleurs pour les yeux.
 */
enum class ColorScheme {
  CARS_ORANGE = 0, ///< Orange + bleu cyan (style Cars)
  SOFT_REALISTIC,      ///< Blanc doux + Ombres bleu profondes (Yeux naturels)
  ELEGANT_BLUE,        ///< Bleu subtil + Reflets bleu pâle (Élégant)
  CARS_ORANGE_V2,      ///< Palette Cars enrichie 8 couleurs
  HUMAN_EYE            ///< Palette réaliste œil humain (8 couleurs)
};

/**
 * @brief Structure de l'état des yeux.
 */
struct EyeState {
  AnimationType currentAnimation;
  uint8_t brightness;
  uint32_t primaryColor;      // Main color (iris/pupil)
  uint32_t secondaryColor;    // Highlight/eyelid color
  uint32_t tertiaryColor;     // Shadow/depth color
  bool autoPlay;
  uint32_t lastAnimationChange;
  uint8_t animationFrame;
};

// ============================================================================
// INITIALIZATION
// ============================================================================

/**
 * Initialize both NeoPixel eye matrices (left and right).
 * Only compiled for ENV_ESP32S3_N16R8.
 */
void init();

// ============================================================================
// BASIC CONTROL
// ============================================================================

/**
 * Set brightness level for both eyes.
 * @param brightness Brightness value (0-255)
 */
void setBrightness(uint8_t brightness);

/**
 * Get current brightness level.
 * @return Current brightness (0-255)
 */
uint8_t getBrightness();

/**
 * Turn off both eyes completely and stop all animations.
 * This sets a special OFF state that persists until another animation is selected.
 */
void turnOff();

/**
 * Turn off both eyes.
 */
void clear();

/**
 * Get a color value from RGB components.
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return 32-bit color value
 */
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);

// ============================================================================
// ANIMATION CONTROL
// ============================================================================

/**
 * Set the current animation for both eyes.
 * @param animation Animation type
 */
void setAnimation(AnimationType animation);

/**
 * Get the current animation type.
 * @return Current animation
 */
AnimationType getAnimation();

/**
 * Set primary color for animations.
 * @param color RGB color value
 */
void setPrimaryColor(uint32_t color);

/**
 * Set secondary color for animations.
 * @param color RGB color value
 */
void setSecondaryColor(uint32_t color);

/**
 * Set tertiary color for animations (shadow/depth effects).
 * @param color RGB color value
 */
void setTertiaryColor(uint32_t color);

/**
 * Apply a predefined color scheme (all 3 colors at once).
 * @param scheme Color scheme preset
 */
void applyColorScheme(ColorScheme scheme);

/**
 * Get the current color scheme.
 * @return Current scheme
 */
ColorScheme getCurrentColorScheme();

/**
 * Enable or disable auto-play mode (random animations).
 * @param enabled True to enable auto-play
 */
void setAutoPlay(bool enabled);

/**
 * Check if auto-play is enabled.
 * @return True if auto-play is enabled
 */
bool isAutoPlayEnabled();

/*
 * Sets Turn Signal duration (ms) for short press
 */
void setSignalDuration(uint16_t durationMs);
uint16_t getSignalDuration();

/**
 * Activate a turn signal or hazard mode.
 * @param type TURN_LEFT, TURN_RIGHT or HAZARD
 * @param longPress If true, stays on until cancelled. If false, turns off after duration.
 */
void toggleTurnSignal(AnimationType type, bool longPress);

/**
 * Load settings from persistent memory (Preferences)
 */
void loadSettings();

/**
 * Save settings to persistent memory (Preferences)
 */
void saveSettings();

/**
 * Update animation state (call in loop).
 * Handles animation frame updates and auto-play.
 */
void update();

// ============================================================================
// DIRECT PIXEL CONTROL
// ============================================================================

/**
 * Set a single pixel on left eye.
 * @param x X coordinate (0-7)
 * @param y Y coordinate (0-7)
 * @param color RGB color value
 */
void setPixelLeft(uint8_t x, uint8_t y, uint32_t color);

/**
 * Set a single pixel on right eye.
 * @param x X coordinate (0-7)
 * @param y Y coordinate (0-7)
 * @param color RGB color value
 */
void setPixelRight(uint8_t x, uint8_t y, uint32_t color);

/**
 * Show all pending pixel changes.
 */
void show();

// ============================================================================
// CUSTOM PATTERN CONTROL
// ============================================================================

/**
 * Set custom pattern for left eye (64 pixels).
 * @param pattern Array of 64 RGB color values
 */
void setCustomPatternLeft(const uint32_t* pattern);

/**
 * Set custom pattern for right eye (64 pixels).
 * @param pattern Array of 64 RGB color values
 */
void setCustomPatternRight(const uint32_t* pattern);

/**
 * Get custom pattern for left eye.
 * @return Pointer to 64-element color array
 */
const uint32_t* getCustomPatternLeft();

/**
 * Get custom pattern for right eye.
 * @return Pointer to 64-element color array
 */
const uint32_t* getCustomPatternRight();

/**
 * Check if custom patterns have been defined.
 * @return True if custom patterns are set
 */
bool hasCustomPatterns();

// ============================================================================
// STATUS LED (GPIO48)
// ============================================================================

/**
 * Set the built-in status LED color (GPIO48).
 * @param color RGB color value
 */
void setStatusLed(uint32_t color);

// ============================================================================
// STATE ACCESS
// ============================================================================

/**
 * Get current eye state (read-only).
 * @return Reference to eye state
 */
const EyeState& getState();

}  // namespace NeoPixel
