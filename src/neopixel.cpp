/**
 * @file neopixel.cpp
 * @brief Implémentation du contrôle des animations et couleurs NeoPixel.
 * @note Utiliser des buffers statiques pour les animations et limiter l'utilisation de String pour optimiser la mémoire.
 */

#include "neopixel.h"
#include "config.h"
#include "board_config.h"

#if defined(ENV_ESP32S3_N16R8)
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

static Adafruit_NeoPixel eyeLeft(NEOPIXEL_MATRIX_COUNT, NEOPIXEL_EYE_LEFT_PIN, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel eyeRight(NEOPIXEL_MATRIX_COUNT, NEOPIXEL_EYE_RIGHT_PIN, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel statusLed(1, NEOPIXEL_STATUS_PIN, NEO_GRB + NEO_KHZ800);
static Preferences prefs;

namespace NeoPixel {

// ============================================================================
// INTERNAL STATE
// ============================================================================

static EyeState g_eyeState = {
  .currentAnimation = AnimationType::IDLE,
  .brightness = NEOPIXEL_BRIGHTNESS,
  .primaryColor = 0x00FF4500,
  .secondaryColor = 0x000099FF,
  .tertiaryColor = 0x00000000,
  .autoPlay = false,
  .lastAnimationChange = 0,
  .animationFrame = 0
};

static ColorScheme g_currentScheme = ColorScheme::CARS_ORANGE;
static unsigned long g_lastFrameTime = 0;
static unsigned long g_nextIdleBlinkTime = 0;
static uint32_t g_palette[8] = {0};

// Signal State
static uint16_t g_signalDurationMs = 3000;   // Default 3s short press
static bool g_signalActive = false;
static bool g_signalLongPress = false;
static unsigned long g_signalStartTime = 0;
static AnimationType g_previousAnimation = AnimationType::IDLE;

// Custom Pattern State
static uint32_t g_customPatternLeft[64] = {0};
static uint32_t g_customPatternRight[64] = {0};
static bool g_hasCustomPatterns = false;

// ============================================================================
// COLOR SCHEME DEFINITIONS
// ============================================================================
// Scheme #1: CARS_ORANGE
const uint32_t SCHEME_CARS_ORANGE[3] = {
  0x00E63500,  // Primary: Intense Red-Orange
  0x00FF5500,  // Secondary: Safety Orange
  0x00400500   // Tertiary: Dark Red-Brown
};

// Scheme #1b: CARS_ORANGE_V2 (8-color enriched palette)
const uint32_t SCHEME_CARS_ORANGE_V2[8] = {
  0x00E64A00,  // C1: Orange chaud (iris)
  0x00CC3C00,  // C2: Ambre soutenu (proche de C1)
  0x00802700,  // C3: Brun-ambre sombre (ombre)
  0x00B85A1A,  // C4: Ambre chaud atténué (reflet, pas blanc)
  0x00994D19,  // C5: Ambre froid/mat (reflet doux)
  0x00401510,  // C6: Brun profond (contraste proche de la palette)
  0x00A63A00,  // C7: Ambre contour plus sombre
  0x00000000   // C8: Noir (pupille)
};

// Scheme #2: SOFT_REALISTIC
const uint32_t SCHEME_SOFT_REALISTIC[3] = {
  0x008B4513,  // Primary: Saddle Brown
  0x00CD853F,  // Secondary: Peru (Terra Cotta)
  0x00251005   // Tertiary: Very dark brown
};

// Scheme #2b: HUMAN_EYE (8-color realistic palette)
const uint32_t SCHEME_HUMAN_EYE[8] = {
  0x00A06C3A,  // C1: Brun clair (iris)
  0x007C5328,  // C2: Brun moyen (proche de C1)
  0x00412A15,  // C3: Brun foncé (ombre iris)
  0x006A7F4F,  // C4: Vert olive doux (nuance proche)
  0x00D9D0C4,  // C5: Blanc chaud atténué (sclère non éclatante)
  0x00BBD0D8,  // C6: Blanc froid atténué (reflet doux)
  0x003A5168,  // C7: Bleu acier plus sombre (reflet moins agressif)
  0x00000000   // C8: Noir profond (pupille)
};

// Scheme #3: ELEGANT_BLUE
const uint32_t SCHEME_ELEGANT_BLUE[3] = {
  0x000033CC,  // Primary: Medium Dark Blue
  0x004499FF,  // Secondary: Azure
  0x00000520   // Tertiary: Very dark blue
};

// ============================================================================
// ANIMATION PATTERNS
// ============================================================================

// --- PATTERN #01: IDLE -----------------------------------------------------
// Frame 0: Wide open
static const uint8_t PATTERN_IDLE_FRAME0[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  51, 50, 11, 10, 10, 11, 50, 51,
  50, 11, 80, 81, 81, 80, 11, 50,
  50, 11, 81, 82, 82, 81, 11, 50,
  51, 50, 11, 10, 10, 11, 50, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};
// Frame 1: Slightly relaxed with shaded lid
static const uint8_t PATTERN_IDLE_FRAME1[64] = {
  0, 0, 71, 71, 71, 71, 0, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  51, 50, 11, 10, 10, 11, 50, 51,
  50, 11, 80, 81, 81, 80, 11, 50,
  50, 11, 81, 82, 82, 81, 11, 50,
  51, 50, 11, 10, 10, 11, 50, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};

// --- PATTERN #02: SLEEP (Closed eye with soft breathing motion) -------------
static const uint8_t PATTERN_SLEEP_FRAME0[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 51, 51, 51, 51, 51, 51, 0,   // Upper eyelid (highlight)
  0, 41, 41, 41, 41, 41, 41, 0,   // Main lid line (coloration)
  0, 41, 41, 41, 41, 41, 41, 0,   // 3D thickness
  0, 51, 51, 51, 51, 51, 51, 0,   // Lower eyelid (highlight)
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const uint8_t PATTERN_SLEEP_FRAME1[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 51, 50, 50, 50, 51, 51, 0,   // Upper eyelid (breathing variation)
  0, 41, 41, 50, 50, 41, 41, 0,   // Slight pulse
  0, 41, 41, 50, 50, 41, 41, 0,   // Sleeping respiration
  0, 51, 50, 50, 50, 51, 51, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// --- PATTERN #03: BLINK -----------------------------------------------------
static const uint8_t PATTERN_BLINK_FRAME0[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  51, 50, 11, 10, 10, 11, 50, 51,
  50, 11, 80, 81, 81, 80, 11, 50,
  50, 11, 81, 82, 82, 81, 11, 50,
  51, 50, 11, 10, 10, 11, 50, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};
static const uint8_t PATTERN_BLINK_FRAME1[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 71, 71, 71, 71, 0, 0,
  0, 51, 50, 50, 50, 50, 51, 0,
  51, 50, 11, 10, 10, 11, 50, 51,
  51, 50, 81, 82, 82, 81, 50, 51,
  0, 51, 50, 50, 50, 50, 51, 0,
  0, 0, 71, 71, 71, 71, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const uint8_t PATTERN_BLINK_FRAME2[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  71, 71, 51, 50, 50, 51, 71, 71,
  71, 71, 50, 50, 50, 50, 71, 71,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// --- PATTERN #04: LOOK_LEFT ------------------------------------------------
// Eyes looking LEFT with blinking pupils (4-point blink)
static const uint8_t PATTERN_LOOK_LEFT_FRAME0[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  51, 41, 11, 50, 51, 71, 71, 0,
  50, 11, 10, 81, 50, 71, 71, 0,    // 4-point pupils: 80, 81
  50, 10, 11, 80, 50, 71, 71, 0,    // alternating
  51, 41, 11, 50, 51, 71, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};
static const uint8_t PATTERN_LOOK_LEFT_FRAME1[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  51, 41, 10, 50, 51, 71, 71, 0,
  50, 81, 80, 81, 50, 71, 71, 0,    // 4-point blink: 81, 80, 81
  50, 80, 81, 80, 50, 71, 71, 0,
  51, 41, 10, 50, 51, 71, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};

// --- PATTERN #05: LOOK_RIGHT -----------------------------------------------
// Eyes looking RIGHT with blinking pupils (4-point blink)
static const uint8_t PATTERN_LOOK_RIGHT_FRAME0[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 71, 51, 41, 11, 50, 51,
  0, 71, 71, 50, 81, 11, 10, 50,    // 4-point pupils: 81, 80
  0, 71, 71, 50, 80, 10, 11, 50,    // alternating
  0, 71, 71, 51, 41, 11, 50, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};
static const uint8_t PATTERN_LOOK_RIGHT_FRAME1[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 71, 51, 41, 10, 50, 51,
  0, 71, 71, 50, 80, 81, 80, 50,    // 4-point blink: 80, 81, 80
  0, 71, 71, 50, 81, 80, 81, 50,
  0, 71, 71, 51, 41, 10, 50, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};

// --- PATTERN #06: LOOK_UP --------------------------------------------------
// Eyes looking UP (LOOK_LEFT rotated 90° clockwise)
static const uint8_t PATTERN_LOOK_UP_FRAME0[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 41, 10, 10, 41, 51, 71,
  51, 50, 11, 11, 10, 11, 50, 51,
  50, 50, 50, 80, 81, 50, 50, 50,
  50, 50, 51, 50, 50, 51, 50, 50,
  51, 50, 71, 71, 71, 71, 50, 51,
  71, 51, 71, 71, 71, 71, 51, 71,
  0, 71, 0, 0, 0, 0, 71, 0
};
static const uint8_t PATTERN_LOOK_UP_FRAME1[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 41, 80, 80, 41, 51, 71,
  51, 50, 10, 80, 81, 10, 50, 51,
  50, 50, 50, 81, 80, 50, 50, 50,
  50, 50, 51, 50, 50, 51, 50, 50,
  51, 50, 71, 71, 71, 71, 50, 51,
  71, 51, 71, 71, 71, 71, 51, 71,
  0, 71, 0, 0, 0, 0, 71, 0
};

// --- PATTERN #07: LOOK_DOWN ------------------------------------------------
// Eyes looking DOWN (LOOK_LEFT rotated 90° counter-clockwise)
static const uint8_t PATTERN_LOOK_DOWN_FRAME0[64] = {
  0, 71, 0, 0, 0, 0, 71, 0,
  71, 51, 71, 71, 71, 71, 51, 71,
  51, 50, 71, 71, 71, 71, 50, 51,
  50, 50, 51, 50, 50, 51, 50, 50,
  50, 50, 50, 81, 80, 50, 50, 50,
  51, 50, 11, 10, 11, 11, 50, 51,
  71, 51, 41, 11, 10, 41, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};
static const uint8_t PATTERN_LOOK_DOWN_FRAME1[64] = {
  0, 71, 0, 0, 0, 0, 71, 0,
  71, 51, 71, 71, 71, 71, 51, 71,
  51, 50, 71, 71, 71, 71, 50, 51,
  50, 50, 51, 50, 50, 51, 50, 50,
  50, 50, 50, 80, 81, 50, 50, 50,
  51, 50, 10, 81, 80, 10, 50, 51,
  71, 51, 41, 80, 80, 41, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};

// --- PATTERN #08: HAPPY (Intense Joy) ------------------------------------
static const uint8_t PATTERN_HAPPY_FRAME0[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  0, 51, 50, 41, 41, 50, 51, 0,     // Paupières hautes, sclère brillante
  51, 41, 11, 10, 10, 11, 41, 51,   // Iris haut (regard émerveillé)
  41, 11, 80, 81, 81, 80, 11, 41,   // Pupille haute
  41, 10, 80, 81, 81, 80, 10, 41,   // Reflet fort
  51, 41, 11, 10, 10, 11, 41, 51,   // Reflet renforcé
  71, 50, 41, 11, 11, 41, 50, 71,   // Bas arrondi (sourire)
  0, 71, 51, 50, 50, 51, 71, 0
};
static const uint8_t PATTERN_HAPPY_FRAME1[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  0, 50, 41, 11, 11, 41, 50, 0,     // Encore plus ouvert
  50, 11, 10, 80, 80, 10, 11, 50,   // Iris très visible
  41, 10, 81, 82, 82, 81, 10, 41,   // Grande pupille joyeuse
  41, 10, 81, 82, 82, 81, 10, 41,   // Brillance maximale
  50, 11, 10, 80, 80, 10, 11, 50,   // Fort contraste
  71, 50, 41, 11, 11, 41, 50, 71,   // Sourire prononcé
  0, 71, 51, 50, 50, 51, 71, 0
};

// --- PATTERN #09: SAD (Droopy eyes) ----------------------------------------
static const uint8_t PATTERN_SAD_FRAME0[64] = {
  0, 71, 0, 0, 0, 0, 71, 0, // Sourcils baissés
  71, 71, 70, 0, 0, 70, 71, 71, // Sourcils avec ombre
  0, 71, 51, 50, 50, 51, 71, 0, // Paupière supérieure tombante
  71, 51, 41, 10, 10, 41, 51, 71,
  50, 41, 10, 80, 80, 10, 41, 50, // Iris plissée
  41, 10, 80, 81, 81, 80, 10, 41,
  71, 51, 51, 41, 41, 51, 51, 71, // Paupière basse active
  0, 0, 71, 51, 51, 71, 0, 0
};
static const uint8_t PATTERN_SAD_FRAME1[64] = {
  71, 0, 71, 0, 0, 71, 0, 71, // Sourcils agités (pleurs)
  70, 70, 70, 70, 70, 70, 70, 70, // Sourcil continu
  0, 71, 51, 50, 50, 51, 71, 0, // Paupière supérieure très tombante
  71, 51, 41, 10, 10, 41, 51, 71,
  50, 41, 10, 81, 81, 10, 41, 50, // Iris encore plus rétrécie
  41, 10, 80, 81, 81, 80, 10, 41,
  70, 70, 70, 70, 70, 70, 70, 70, // Paupière basse tremblante
  0, 71, 51, 71, 71, 51, 71, 0   // Larmes suggérées
};

// --- PATTERN #10: ANGRY ----------------------------------------------------
static const uint8_t PATTERN_ANGRY_FRAME0[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 71, 51, 51, 71, 0, 0,
  0, 0, 50, 41, 41, 50, 71, 0,
  51, 50, 80, 81, 81, 80, 50, 51,
  50, 41, 80, 82, 82, 80, 41, 50,
  51, 41, 10, 80, 80, 10, 41, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};
static const uint8_t PATTERN_ANGRY_FRAME1[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 71, 51, 51, 71, 0, 0,
  0, 0, 50, 41, 41, 50, 71, 0,
  51, 50, 80, 81, 81, 80, 50, 51,
  50, 41, 80, 82, 82, 80, 41, 50,
  0, 51, 50, 41, 41, 50, 51, 0,
  0, 0, 51, 50, 50, 51, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// --- PATTERN #11: SURPRISED ------------------------------------------------
// Very wide open, small iris
static const uint8_t PATTERN_SURPRISED_FRAME0[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  51, 50, 11, 10, 10, 11, 50, 51,
  50, 41, 10, 11, 11, 10, 41, 50, // Small iris
  50, 41, 10, 11, 11, 10, 41, 50,
  51, 50, 11, 10, 10, 11, 50, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};
// Pulse pupil size
static const uint8_t PATTERN_SURPRISED_FRAME1[64] = {
  0, 71, 51, 50, 50, 51, 71, 0,
  71, 51, 50, 50, 50, 50, 51, 71,
  51, 50, 11, 10, 10, 11, 50, 51,
  50, 41, 10, 30, 30, 10, 41, 50, // Very small pupil
  50, 41, 10, 30, 30, 10, 41, 50,
  51, 50, 11, 10, 10, 11, 50, 51,
  71, 51, 50, 50, 50, 50, 51, 71,
  0, 71, 51, 50, 50, 51, 71, 0
};


// --- PATTERN #16: ARROW RIGHT (Turn Signal) --------------------------------
static const uint8_t PATTERN_ARROW_RIGHT[64] = {
  0, 0, 0, 0, 0, 100, 0, 0,
  0, 0, 0, 0, 0, 100, 100, 0,
  100, 100, 100, 100, 100, 100, 100, 0,
  100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 0,
  0, 0, 0, 0, 0, 100, 100, 0,
  0, 0, 0, 0, 0, 100, 0, 0
};

// --- PATTERN #17: ARROW LEFT (Turn Signal) --------------------------------
static const uint8_t PATTERN_ARROW_LEFT[64] = {
  0, 0, 100, 0, 0, 0, 0, 0,
  0, 100, 100, 0, 0, 0, 0, 0,
  0, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100,
  0, 100, 100, 100, 100, 100, 100, 100,
  0, 100, 100, 0, 0, 0, 0, 0,
  0, 0, 100, 0, 0, 0, 0, 0
};

// --- PATTERN #18: HAZARD (Both Arrows Out) ---------------------------------
// Reuse ARROW_LEFT for Left Eye and ARROW_RIGHT for Right Eye

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

static uint8_t xyToIndex(uint8_t x, uint8_t y) {
  if (x >= NEOPIXEL_MATRIX_WIDTH || y >= NEOPIXEL_MATRIX_HEIGHT) return 0;
  return y * NEOPIXEL_MATRIX_WIDTH + x;
}

static uint32_t dimColor(uint32_t color, uint8_t percent) {
  if (percent >= 100) return color;
  if (percent == 0) return 0;
  
  uint8_t r = (uint8_t)(color >> 16);
  uint8_t g = (uint8_t)(color >> 8);
  uint8_t b = (uint8_t)color;
  
  r = (r * percent) / 100;
  g = (g * percent) / 100;
  b = (b * percent) / 100;
  
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

// Additional perceived brightness adjustment for very low brightness settings.
// This reduces overly bright "white-ish" colors more than dark hues when brightness < 64.
static uint32_t adjustPerceivedBrightness(uint32_t color, uint8_t brightness) {
  if (brightness >= 64) return color;
  uint8_t r = (uint8_t)(color >> 16);
  uint8_t g = (uint8_t)(color >> 8);
  uint8_t b = (uint8_t)color;

  // Approximate luminance (weights sum ~100): Y ≈ 0.21R + 0.72G + 0.07B
  uint16_t lum = (uint16_t)((uint16_t)r * 21 + (uint16_t)g * 72 + (uint16_t)b * 7) / 100; // 0..255
  uint16_t low = (uint16_t)(64 - brightness); // 0..64

  // Extra dimming in per-thousand based on how low brightness is and luminance.
  // Max extra dim ≈ 0.6 (600/1000) when brightness→0 and color≈white (lum≈255).
  uint32_t extra = (uint32_t)low * (uint32_t)lum * 600u; // numerator
  uint32_t denom = 64u * 255u;                           // 16320
  uint16_t extraTh = (uint16_t)(extra / denom);          // 0..~600
  uint16_t scaleTh = (uint16_t)(1000u - extraTh);        // 1000..400
  if (scaleTh < 300) scaleTh = 300;                      // clamp min to 0.3

  r = (uint8_t)(((uint32_t)r * scaleTh) / 1000u);
  g = (uint8_t)(((uint32_t)g * scaleTh) / 1000u);
  b = (uint8_t)(((uint32_t)b * scaleTh) / 1000u);

  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

static void fillPaletteFromLegacy(uint32_t primary, uint32_t secondary, uint32_t tertiary) {
  g_palette[0] = primary;                     // C1 vibrant
  g_palette[1] = secondary;                   // C2 accent
  g_palette[2] = tertiary;                    // C3 shadow
  g_palette[3] = dimColor(secondary, 70);     // C4 warm highlight
  g_palette[4] = dimColor(secondary, 40);     // C5 cool highlight
  g_palette[5] = dimColor(tertiary, 70);      // C6 deep shadow
  g_palette[6] = dimColor(tertiary, 40);      // C7 outline
  g_palette[7] = 0x00000000;                  // C8 black
}

static void setPaletteFromScheme(ColorScheme scheme) {
  switch (scheme) {
    case ColorScheme::CARS_ORANGE:
      fillPaletteFromLegacy(SCHEME_CARS_ORANGE[0], SCHEME_CARS_ORANGE[1], SCHEME_CARS_ORANGE[2]);
      break;
    case ColorScheme::SOFT_REALISTIC:
      fillPaletteFromLegacy(SCHEME_SOFT_REALISTIC[0], SCHEME_SOFT_REALISTIC[1], SCHEME_SOFT_REALISTIC[2]);
      break;
    case ColorScheme::ELEGANT_BLUE:
      fillPaletteFromLegacy(SCHEME_ELEGANT_BLUE[0], SCHEME_ELEGANT_BLUE[1], SCHEME_ELEGANT_BLUE[2]);
      break;
    case ColorScheme::CARS_ORANGE_V2:
      memcpy(g_palette, SCHEME_CARS_ORANGE_V2, sizeof(g_palette));
      g_eyeState.primaryColor = g_palette[0];
      g_eyeState.secondaryColor = g_palette[1];
      g_eyeState.tertiaryColor = g_palette[2];
      break;
    case ColorScheme::HUMAN_EYE:
      memcpy(g_palette, SCHEME_HUMAN_EYE, sizeof(g_palette));
      g_eyeState.primaryColor = g_palette[0];
      g_eyeState.secondaryColor = g_palette[1];
      g_eyeState.tertiaryColor = g_palette[2];
      break;
  }
}

static void drawPattern(Adafruit_NeoPixel& eye, const uint8_t* pattern, const uint32_t* palette) {
  for (uint8_t y = 0; y < NEOPIXEL_MATRIX_HEIGHT; y++) {
    for (uint8_t x = 0; x < NEOPIXEL_MATRIX_WIDTH; x++) {
      uint8_t idx = xyToIndex(x, y);
      uint8_t code = pattern[y * NEOPIXEL_MATRIX_WIDTH + x];
      uint32_t finalColor = 0;

      if (code == 0) {
        finalColor = 0;
      } else if (code == 100) {
        finalColor = palette[0];
      } else if (code >= 1 && code <= 8) {
        finalColor = palette[code - 1];
      } else if (code >= 10 && code < 90) {
        uint8_t tens = code / 10;
        uint8_t idxPalette = (tens >= 1) ? (uint8_t)(tens - 1) : 0;
        if (idxPalette > 7) idxPalette = 7;
        uint8_t mod = code % 10;
        uint8_t percent = 100;
        if (mod == 1) percent = 70;
        else if (mod == 2) percent = 40;
        else if (mod == 3) percent = 20;
        else if (mod == 4) percent = 10;
        finalColor = dimColor(palette[idxPalette], percent);
      } else {
        finalColor = 0;
      }

      // Apply perceived brightness adjustment for low global brightness
      finalColor = adjustPerceivedBrightness(finalColor, g_eyeState.brightness);
      eye.setPixelColor(idx, finalColor);
    }
  }
}

static const uint8_t* getPatternForAnimation(AnimationType anim, uint8_t frame) {
  switch (anim) {
    case AnimationType::IDLE:
      // More lively idle: slight movements every few frames
      // Cycle of 20 frames (1 second)
      if (frame % 20 < 10) return PATTERN_IDLE_FRAME0;
      return PATTERN_IDLE_FRAME1;
      
    case AnimationType::BLINK:
      if (frame == 0) return PATTERN_BLINK_FRAME0; 
      if (frame == 1) return PATTERN_BLINK_FRAME1; 
      if (frame == 2) return PATTERN_BLINK_FRAME2; 
      if (frame == 3) return PATTERN_BLINK_FRAME2; 
      if (frame == 4) return PATTERN_BLINK_FRAME1; 
      return PATTERN_IDLE_FRAME0; 
      
    case AnimationType::LOOK_LEFT:
      if (frame % 10 < 5) return PATTERN_LOOK_LEFT_FRAME0;
      return PATTERN_LOOK_LEFT_FRAME1;

    case AnimationType::LOOK_RIGHT:
      if (frame % 10 < 5) return PATTERN_LOOK_RIGHT_FRAME0;
      return PATTERN_LOOK_RIGHT_FRAME1;

    case AnimationType::LOOK_UP:
      if (frame % 10 < 5) return PATTERN_LOOK_UP_FRAME0;
      return PATTERN_LOOK_UP_FRAME1;

    case AnimationType::LOOK_DOWN:
      if (frame % 10 < 5) return PATTERN_LOOK_DOWN_FRAME0;
      return PATTERN_LOOK_DOWN_FRAME1;
    
    case AnimationType::HAPPY:
      if (frame % 8 < 4) return PATTERN_HAPPY_FRAME0;
      return PATTERN_HAPPY_FRAME1;

    case AnimationType::ANGRY:
      if (frame % 8 < 4) return PATTERN_ANGRY_FRAME0;
      return PATTERN_ANGRY_FRAME1;
    
    case AnimationType::SAD:
      if (frame % 16 < 8) return PATTERN_SAD_FRAME0;
      return PATTERN_SAD_FRAME1;

    case AnimationType::SURPRISED:
      if (frame % 6 < 3) return PATTERN_SURPRISED_FRAME0;
      return PATTERN_SURPRISED_FRAME1;

    case AnimationType::SLEEP:
      if (frame % 40 < 20) return PATTERN_SLEEP_FRAME0;
      return PATTERN_SLEEP_FRAME1;

    case AnimationType::TURN_LEFT:
      if (frame % 10 < 5) return PATTERN_ARROW_LEFT;
      return NULL; // Blink off

    case AnimationType::TURN_RIGHT:
      if (frame % 10 < 5) return PATTERN_ARROW_RIGHT;
      return NULL; // Blink off
    
    case AnimationType::HAZARD:
       // Handled in main loop logic as mixture, but pattern getter needs base
      if (frame % 10 < 5) return PATTERN_ARROW_LEFT; // Default placeholder
      return NULL; 

    default:
      return PATTERN_IDLE_FRAME0;
  }
}

// ============================================================================
// PUBLIC API IMPLEMENTATION
// ============================================================================

void loadSettings() {
  prefs.begin("carseyes", true); // Read-only mode
  g_eyeState.brightness = prefs.getUChar("bright", NEOPIXEL_BRIGHTNESS);
  g_eyeState.primaryColor = prefs.getUInt("col_pri", 0x00FF4500);
  g_eyeState.secondaryColor = prefs.getUInt("col_sec", 0x000099FF);
  g_eyeState.tertiaryColor = prefs.getUInt("col_ter", 0x00000000);
  g_currentScheme = (ColorScheme)prefs.getInt("scheme", (int)ColorScheme::CARS_ORANGE);
  g_signalDurationMs = prefs.getUShort("sig_dur", 3000);
  // Initialize palette from stored colors first (legacy compatibility)
  fillPaletteFromLegacy(g_eyeState.primaryColor, g_eyeState.secondaryColor, g_eyeState.tertiaryColor);
  // If scheme supports 8 colors, override with scheme palette
  if (g_currentScheme == ColorScheme::CARS_ORANGE_V2 || g_currentScheme == ColorScheme::HUMAN_EYE) {
    setPaletteFromScheme(g_currentScheme);
  }
  
  // Load saved mode if not idle
  int savedAnim = prefs.getInt("anim", (int)AnimationType::IDLE);
  g_eyeState.currentAnimation = (AnimationType)savedAnim;
  
  // Load custom patterns
  g_hasCustomPatterns = prefs.getBool("has_custom", false);
  if (g_hasCustomPatterns) {
    // Load left pattern
    prefs.getBytes("custom_l", g_customPatternLeft, 64 * sizeof(uint32_t));
    // Load right pattern
    prefs.getBytes("custom_r", g_customPatternRight, 64 * sizeof(uint32_t));
  }
  
  prefs.end();
}

void saveSettings() {
  prefs.begin("carseyes", false); // RW mode
  prefs.putUChar("bright", g_eyeState.brightness);
  prefs.putUInt("col_pri", g_eyeState.primaryColor);
  prefs.putUInt("col_sec", g_eyeState.secondaryColor);
  prefs.putUInt("col_ter", g_eyeState.tertiaryColor);
  prefs.putInt("scheme", (int)g_currentScheme);
  prefs.putUShort("sig_dur", g_signalDurationMs);
  prefs.putInt("anim", (int)g_eyeState.currentAnimation);
  
  // Save custom patterns
  prefs.putBool("has_custom", g_hasCustomPatterns);
  if (g_hasCustomPatterns) {
    prefs.putBytes("custom_l", g_customPatternLeft, 64 * sizeof(uint32_t));
    prefs.putBytes("custom_r", g_customPatternRight, 64 * sizeof(uint32_t));
  }
  
  prefs.end();
}

void setSignalDuration(uint16_t durationMs) {
  g_signalDurationMs = durationMs;
  saveSettings();
}

uint16_t getSignalDuration() { return g_signalDurationMs; }

void toggleTurnSignal(AnimationType type, bool longPress) {
  // If requesting same type that is already active
  if (g_signalActive && g_eyeState.currentAnimation == type) {
      // If was short, now long -> Upgrade to long
      if (!g_signalLongPress && longPress) {
         g_signalLongPress = true;
         return;
      }
      // If was active (long or short) and new request is canceling or re-triggering?
      // Rule: If already active, cancel it
      setAnimation(g_previousAnimation); 
      g_signalActive = false;
      return;
  }
  
  // If not active, start new signal
  if (!g_signalActive) {
      g_previousAnimation = g_eyeState.currentAnimation;
  }
  
  g_signalActive = true;
  g_signalLongPress = longPress;
  g_signalStartTime = millis();
  
  // Directly set animation without verifying 'g_eyeState.currentAnimation' persistence for saving
  // We don't want to save temporary signal state
  g_eyeState.currentAnimation = type;
  g_eyeState.animationFrame = 0;
  g_lastFrameTime = millis();
  
  update();
}

void setAnimation(AnimationType animation) {
  g_eyeState.currentAnimation = animation;
  g_eyeState.animationFrame = 0;
  
  // Clear signal flag if we manually change animation
  if (animation != AnimationType::TURN_LEFT && 
      animation != AnimationType::TURN_RIGHT && 
      animation != AnimationType::HAZARD) {
        g_signalActive = false;
  }
  
  // FIX: Force immediate update logic
  g_lastFrameTime = 0;
  
  // FIX: Reset AutoPlay timer to prevent immediate override
  if (g_eyeState.autoPlay) {
      g_eyeState.lastAnimationChange = millis();
  }
  
  // Persist choice (except signals)
  if (!g_signalActive) {
    saveSettings(); 
  }
  
  update();
}

/**
 * @brief Initialise les matrices NeoPixel et l'état interne.
 */
void init() {
  // Load settings first
  loadSettings();

  eyeLeft.begin();
  eyeLeft.clear();
  eyeLeft.setBrightness(g_eyeState.brightness);
  
  eyeRight.begin();
  eyeRight.clear();
  eyeRight.setBrightness(g_eyeState.brightness);
  
  statusLed.begin();
  statusLed.clear();
  statusLed.setBrightness(64);
  
  // Always start with IDLE animation at boot regardless of saved state
  g_eyeState.currentAnimation = AnimationType::IDLE;
  update();
}

void setBrightness(uint8_t brightness) {
  g_eyeState.brightness = brightness;
  eyeLeft.setBrightness(brightness);
  eyeRight.setBrightness(brightness);
  show();
  saveSettings();
}

uint8_t getBrightness() { return g_eyeState.brightness; }
void clear() { eyeLeft.clear(); eyeRight.clear(); show(); }
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) { return eyeLeft.Color(r,g,b); }
AnimationType getAnimation() { return g_eyeState.currentAnimation; }

void setPrimaryColor(uint32_t color) {
  g_eyeState.primaryColor = color;
  g_palette[0] = color;
  saveSettings();
  update();
}
void setSecondaryColor(uint32_t color) {
  g_eyeState.secondaryColor = color;
  g_palette[1] = color;
  saveSettings();
  update();
}
void setTertiaryColor(uint32_t color) {
  g_eyeState.tertiaryColor = color;
  g_palette[2] = color;
  saveSettings();
  update();
}

void applyColorScheme(ColorScheme scheme) {
  g_currentScheme = scheme;
  setPaletteFromScheme(scheme);
  saveSettings();
  update();
}
ColorScheme getCurrentColorScheme() { return g_currentScheme; }

void setAutoPlay(bool enabled) {
  g_eyeState.autoPlay = enabled;
  if(enabled) g_eyeState.lastAnimationChange = millis();
}
bool isAutoPlayEnabled() { return g_eyeState.autoPlay; }

void turnOff() {
  g_eyeState.autoPlay = false;
  g_signalActive = false;
  g_eyeState.currentAnimation = AnimationType::OFF;
  eyeLeft.clear();
  eyeRight.clear();
  eyeLeft.show();
  eyeRight.show();
}

void update() {
  const unsigned long now = millis();
  
  // If LEDs are OFF, do nothing until another animation is selected
  if (g_eyeState.currentAnimation == AnimationType::OFF) {
    return;
  }
  
  // Signal Logic (Timer)
  if (g_signalActive && !g_signalLongPress) {
      if (now - g_signalStartTime > g_signalDurationMs) {
          // Time expired, turn off signal
          setAnimation(g_previousAnimation);
          g_signalActive = false;
          return;
      }
  }

  // AutoPlay Logic (Disable if signal active)
  if (g_eyeState.autoPlay && !g_signalActive) {
    const unsigned long elapsed = now - g_eyeState.lastAnimationChange;
    // Reduce change interval to avoid "freeze" feeling
    if (elapsed > 2000) {
       AnimationType animations[] = {
        AnimationType::IDLE, AnimationType::BLINK,
        AnimationType::LOOK_LEFT, AnimationType::LOOK_RIGHT,
        AnimationType::LOOK_UP, AnimationType::LOOK_DOWN,
        AnimationType::HAPPY, AnimationType::ANGRY,
        AnimationType::SAD, AnimationType::SURPRISED,
        AnimationType::SLEEP
       };
       int idx = random(0, 11);
       setAnimation(animations[idx]);
       g_eyeState.lastAnimationChange = now;
       
       // If it's IDLE, reduce time so it doesn't linger too long
       if (animations[idx] == AnimationType::IDLE) {
           g_eyeState.lastAnimationChange = now - 1000;
       }
    }
  } else {
    // Manual Mode Logic: Random Blink in IDLE (Only if not in signal mode)
    if (!g_signalActive && g_eyeState.currentAnimation == AnimationType::IDLE) {
      if (g_nextIdleBlinkTime == 0) {
        g_nextIdleBlinkTime = now + random(3000, 8000);
      }
      if (now > g_nextIdleBlinkTime) {
        setAnimation(AnimationType::BLINK);
        g_nextIdleBlinkTime = now + random(3000, 8000);
      }
    }
  }

  // Frame Logic
  if (now - g_lastFrameTime >= 50) {
    g_lastFrameTime = now;
    g_eyeState.animationFrame++;
    
    AnimationType anim = g_eyeState.currentAnimation;
    
    if (anim == AnimationType::BLINK && g_eyeState.animationFrame > 5) {
       // Always return to IDLE after blinking (whether AutoPlay or Logic)
       setAnimation(AnimationType::IDLE);
       // Note: setAnimation resets animationFrame to 0
       return; 
    }
    
    if (g_eyeState.animationFrame > 200) g_eyeState.animationFrame = 0;
    
    // Handle CUSTOM pattern separately
    if (anim == AnimationType::CUSTOM && g_hasCustomPatterns) {
      // Display custom patterns directly (no animation frames)
      eyeLeft.clear();
      eyeRight.clear();
      
      // Apply custom pattern left
      for (int i = 0; i < 64; i++) {
        eyeLeft.setPixelColor(i, g_customPatternLeft[i]);
      }
      
      // Apply custom pattern right
      for (int i = 0; i < 64; i++) {
        eyeRight.setPixelColor(i, g_customPatternRight[i]);
      }
      
      show();
      return;
    }

    const uint8_t* patternLeft = getPatternForAnimation(anim, g_eyeState.animationFrame);
    const uint8_t* patternRight = patternLeft; // Default same
    
    // Pattern Selection overrides
    if (anim == AnimationType::WINK_LEFT) {
       patternLeft = getPatternForAnimation(AnimationType::BLINK, g_eyeState.animationFrame);
       patternRight = getPatternForAnimation(AnimationType::IDLE, g_eyeState.animationFrame);
    } else if (anim == AnimationType::WINK_RIGHT) {
       patternLeft = getPatternForAnimation(AnimationType::IDLE, g_eyeState.animationFrame);
       patternRight = getPatternForAnimation(AnimationType::BLINK, g_eyeState.animationFrame);
    } else if (anim == AnimationType::TURN_LEFT) {
       patternLeft = getPatternForAnimation(AnimationType::TURN_LEFT, g_eyeState.animationFrame);
       // Keep Right Eye Idle
       patternRight = getPatternForAnimation(AnimationType::IDLE, g_eyeState.animationFrame);
    } else if (anim == AnimationType::TURN_RIGHT) {
       // Keep Left Eye Idle
       patternLeft = getPatternForAnimation(AnimationType::IDLE, g_eyeState.animationFrame);
       patternRight = getPatternForAnimation(AnimationType::TURN_RIGHT, g_eyeState.animationFrame);
    } else if (anim == AnimationType::HAZARD) {
       // Use Left Arrow on Left, Right Arrow on Right
       patternLeft = getPatternForAnimation(AnimationType::TURN_LEFT, g_eyeState.animationFrame);
       patternRight = getPatternForAnimation(AnimationType::TURN_RIGHT, g_eyeState.animationFrame);
    }
    
    // Draw Left
    if (patternLeft) {
       uint32_t paletteLeft[8];
       memcpy(paletteLeft, g_palette, sizeof(paletteLeft));
       if (anim == AnimationType::TURN_LEFT || anim == AnimationType::TURN_RIGHT || anim == AnimationType::HAZARD) {
          paletteLeft[0] = 0x00FFBF00; // Amber for turn signals
       }
       drawPattern(eyeLeft, patternLeft, paletteLeft);
    } else {
       eyeLeft.clear();
    }
    
    // Draw Right
    if (patternRight) {
       uint32_t paletteRight[8];
       memcpy(paletteRight, g_palette, sizeof(paletteRight));
       if (anim == AnimationType::TURN_LEFT || anim == AnimationType::TURN_RIGHT || anim == AnimationType::HAZARD) {
          paletteRight[0] = 0x00FFBF00; // Amber for turn signals
       }
       drawPattern(eyeRight, patternRight, paletteRight);
    } else {
       eyeRight.clear();
    }
    
    show();
  }
}

void setCustomPatternLeft(const uint32_t* pattern) {
  if (pattern) {
    memcpy(g_customPatternLeft, pattern, 64 * sizeof(uint32_t));
    g_hasCustomPatterns = true;
    saveSettings();
  }
}

void setCustomPatternRight(const uint32_t* pattern) {
  if (pattern) {
    memcpy(g_customPatternRight, pattern, 64 * sizeof(uint32_t));
    g_hasCustomPatterns = true;
    saveSettings();
  }
}

const uint32_t* getCustomPatternLeft() {
  return g_customPatternLeft;
}

const uint32_t* getCustomPatternRight() {
  return g_customPatternRight;
}

bool hasCustomPatterns() {
  return g_hasCustomPatterns;
}

void setPixelLeft(uint8_t x, uint8_t y, uint32_t c) { if(x<8&&y<8) eyeLeft.setPixelColor(xyToIndex(x,y),c); }
void setPixelRight(uint8_t x, uint8_t y, uint32_t c) { if(x<8&&y<8) eyeRight.setPixelColor(xyToIndex(x,y),c); }
void show() { eyeLeft.show(); eyeRight.show(); }
void setStatusLed(uint32_t c) { statusLed.setPixelColor(0,c); statusLed.show(); }
const EyeState& getState() { return g_eyeState; }

} // namespace

#else
namespace NeoPixel {
static EyeState g_d = {};
void init() {}
void setBrightness(uint8_t) {}
uint8_t getBrightness() { return 0; }
void clear() {}
void turnOff() {}
uint32_t makeColor(uint8_t,uint8_t,uint8_t) { return 0; }
void setAnimation(AnimationType) {}
AnimationType getAnimation() { return AnimationType::IDLE; }
void setPrimaryColor(uint32_t) {}
void setSecondaryColor(uint32_t) {}
void setTertiaryColor(uint32_t) {}
void applyColorScheme(ColorScheme) {}
ColorScheme getCurrentColorScheme() { return ColorScheme::CARS_ORANGE; }
void setAutoPlay(bool) {}
bool isAutoPlayEnabled() { return false; }
void update() {}
void setPixelLeft(uint8_t,uint8_t,uint32_t) {}
void setPixelRight(uint8_t,uint8_t,uint32_t) {}
void show() {}
void setStatusLed(uint32_t) {}
const EyeState& getState() { return g_d; }
}
#endif
