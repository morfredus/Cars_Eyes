#pragma once

#include <Arduino.h>

namespace NeoPixel {

// ============================================================================
// TYPES ET STRUCTURES
// ============================================================================

/**
 * Eye animation types
 */
enum class AnimationType {
  IDLE = 0,        // Eyes open and idle
  BLINK,           // Quick blink
  WINK_LEFT,       // Wink left eye
  WINK_RIGHT,      // Wink right eye
  LOOK_LEFT,       // Look to the left
  LOOK_RIGHT,      // Look to the right
  LOOK_UP,         // Look up
  LOOK_DOWN,       // Look down
  HAPPY,           // Happy expression
  SAD,             // Sad expression
  ANGRY,           // Angry expression
  SURPRISED,       // Surprised expression
  SLEEP,           // Sleeping (eyes closed)
  TURN_LEFT,       // Arrow Left (Turn Signal)
  TURN_RIGHT,      // Arrow Right (Turn Signal)
  HAZARD,          // Hazard lights
  CUSTOM           // Custom pattern from web UI
};

/**
 * Color scheme presets for Eyes
 * Each scheme has complementary colors for realistic appearance
 */
enum class ColorScheme {
  CARS_ORANGE = 0,     // Orange primary + Cyan blue highlights (Cars style)
  SOFT_REALISTIC,      // Soft white + Deep blue shadows (Natural eyes)
  ELEGANT_BLUE         // Subtle blue + Pale blue highlights (Elegant)
};

/**
 * Eye state structure
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
