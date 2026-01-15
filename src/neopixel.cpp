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

// Scheme #2: SOFT_REALISTIC
const uint32_t SCHEME_SOFT_REALISTIC[3] = {
  0x008B4513,  // Primary: Saddle Brown
  0x00CD853F,  // Secondary: Peru (Terra Cotta)
  0x00251005   // Tertiary: Very dark brown
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
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 21, 12, 11, 11, 12, 21, 23,
  22, 12, 10, 31, 31, 10, 12, 22,
  22, 12, 30, 31, 31, 30, 12, 22,
  23, 21, 12, 11, 11, 12, 21, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};
// Frame 1: Slightly relaxed with shaded lid
static const uint8_t PATTERN_IDLE_FRAME1[64] = {
  0, 0, 33, 33, 33, 33, 0, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 21, 12, 11, 11, 12, 21, 23,
  22, 12, 10, 31, 31, 10, 12, 22,
  22, 12, 30, 31, 31, 30, 12, 22,
  23, 21, 12, 11, 11, 12, 21, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};

// --- PATTERN #02: SLEEP (Closed eye with soft breathing motion) -------------
static const uint8_t PATTERN_SLEEP_FRAME0[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 33, 23, 23, 33, 0, 0,
  0, 23, 22, 21, 21, 22, 23, 0,
  33, 23, 21, 11, 11, 21, 23, 33,
  23, 21, 11, 10, 10, 11, 21, 23,
  0, 0, 32, 21, 21, 32, 0, 0,
  0, 0, 0, 30, 30, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const uint8_t PATTERN_SLEEP_FRAME1[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 33, 23, 23, 33, 0, 0,
  0, 23, 22, 21, 21, 22, 23, 0,
  33, 23, 21, 11, 11, 21, 23, 33,
  23, 21, 11, 10, 10, 11, 21, 23,
  0, 0, 32, 21, 21, 32, 0, 0,
  0, 0, 0, 30, 30, 0, 0, 0
};

// --- PATTERN #03: BLINK -----------------------------------------------------
static const uint8_t PATTERN_BLINK_FRAME0[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 21, 12, 11, 11, 12, 21, 23,
  22, 12, 10, 31, 31, 10, 12, 22,
  22, 12, 30, 31, 31, 30, 12, 22,
  23, 21, 12, 11, 11, 12, 21, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};
static const uint8_t PATTERN_BLINK_FRAME1[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 33, 33, 33, 33, 0, 0,
  0, 23, 22, 21, 21, 22, 23, 0,
  23, 21, 12, 11, 11, 12, 21, 23,
  23, 21, 30, 31, 31, 30, 21, 23,
  0, 23, 22, 21, 21, 22, 23, 0,
  0, 0, 33, 33, 33, 33, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const uint8_t PATTERN_BLINK_FRAME2[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  33, 33, 23, 22, 22, 23, 33, 33,
  33, 33, 22, 21, 21, 22, 33, 33,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// --- PATTERN #04: LOOK_LEFT ------------------------------------------------
static const uint8_t PATTERN_LOOK_LEFT_FRAME0[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 12, 11, 12, 22, 23, 33, 0,
  22, 10, 31, 31, 21, 23, 33, 0,
  22, 30, 31, 30, 21, 23, 33, 0,
  23, 12, 11, 12, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};
static const uint8_t PATTERN_LOOK_LEFT_FRAME1[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 11, 10, 11, 22, 23, 33, 0,
  22, 31, 30, 31, 21, 23, 33, 0,
  22, 30, 31, 30, 21, 23, 33, 0,
  23, 11, 10, 11, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};

// --- PATTERN #05: LOOK_RIGHT -----------------------------------------------
static const uint8_t PATTERN_LOOK_RIGHT_FRAME0[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 12, 11, 12, 23,
  0, 33, 23, 21, 10, 31, 10, 22,
  0, 33, 23, 21, 30, 31, 30, 22,
  0, 33, 23, 22, 12, 11, 12, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};
static const uint8_t PATTERN_LOOK_RIGHT_FRAME1[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 11, 10, 11, 23,
  0, 33, 23, 21, 31, 30, 31, 22,
  0, 33, 23, 21, 30, 31, 30, 22,
  0, 33, 23, 22, 11, 10, 11, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};

// --- PATTERN #06: LOOK_UP --------------------------------------------------
// Mirrored from LOOK_DOWN
static const uint8_t PATTERN_LOOK_UP_FRAME0[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 21, 12, 11, 11, 12, 21, 23, // Iris top
  22, 12, 10, 31, 31, 10, 12, 22, // Pupil row
  22, 12, 30, 31, 31, 30, 12, 22, // Lower pupil row
  23, 22, 21, 21, 21, 21, 22, 23,
  0, 33, 23, 22, 22, 23, 33, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const uint8_t PATTERN_LOOK_UP_FRAME1[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 21, 12, 11, 11, 12, 21, 33,
  22, 10, 31, 31, 31, 31, 10, 22,
  21, 12, 10, 31, 31, 10, 12, 21,
  22, 21, 21, 21, 21, 21, 21, 22,
  23, 22, 21, 21, 21, 21, 22, 23,
  0, 33, 23, 22, 22, 23, 33, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// --- PATTERN #07: LOOK_DOWN ------------------------------------------------
static const uint8_t PATTERN_LOOK_DOWN_FRAME0[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 33, 23, 22, 22, 23, 33, 0,
  23, 22, 21, 21, 21, 21, 22, 23,
  22, 12, 10, 31, 31, 10, 12, 22,
  22, 12, 30, 31, 31, 30, 12, 22,
  23, 21, 12, 11, 11, 12, 21, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};
static const uint8_t PATTERN_LOOK_DOWN_FRAME1[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 33, 23, 22, 22, 23, 33, 0,
  23, 22, 21, 21, 21, 21, 22, 23,
  22, 21, 21, 21, 21, 21, 21, 22,
  22, 12, 10, 31, 31, 10, 12, 22,
  23, 12, 30, 31, 31, 30, 12, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};

// --- PATTERN #08: HAPPY (Intense Joy) ------------------------------------
static const uint8_t PATTERN_HAPPY_FRAME0[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  0, 22, 21, 20, 20, 21, 22, 0,     // Paupières hautes = yeux très ouverts
  22, 20, 12, 11, 11, 12, 20, 22,   // Iris haut (regard émerveillé)
  21, 12, 31, 30, 30, 31, 12, 21,   // Pupille haute
  21, 11, 30, 33, 33, 30, 11, 21,   // Pupille brillante (joie)
  22, 12, 31, 30, 30, 31, 12, 22,   // Reflet fort
  33, 21, 20, 12, 12, 20, 21, 33,   // Bas arrondi (sourire)
  0, 33, 23, 22, 22, 23, 33, 0      // Paupière basse relevée
};
static const uint8_t PATTERN_HAPPY_FRAME1[64] = {
  0, 33, 22, 21, 21, 22, 33, 0,
  0, 21, 20, 12, 12, 20, 21, 0,     // Encore plus ouvert
  21, 12, 11, 10, 10, 11, 12, 21,   // Iris très visible
  20, 11, 31, 30, 30, 31, 11, 20,   // Grande pupille joyeuse
  20, 10, 30, 33, 33, 30, 10, 20,   // Brillance maximale
  21, 11, 31, 30, 30, 31, 11, 21,   // Fort contraste
  22, 20, 12, 11, 11, 12, 20, 22,   // Sourire prononcé
  0, 22, 21, 20, 20, 21, 22, 0      // Yeux complètement ouverts
};

// --- PATTERN #09: SAD (Droopy eyes) ----------------------------------------
static const uint8_t PATTERN_SAD_FRAME0[64] = {
  0, 33, 0, 0, 0, 0, 33, 0, // Sourcils baissés vers le bas (côté interne)
  33, 32, 31, 0, 0, 31, 32, 33, // Sourcils épais avec ombre
  0, 23, 21, 22, 22, 21, 23, 0, // Paupière supérieure tombante
  23, 21, 20, 11, 11, 20, 21, 23,
  22, 20, 11, 30, 30, 11, 20, 22, // Iris plissée, fermée
  21, 20, 31, 30, 30, 31, 20, 21,
  33, 22, 22, 21, 21, 22, 22, 33, // Paupière basse très active
  0, 0, 33, 23, 23, 33, 0, 0
};
static const uint8_t PATTERN_SAD_FRAME1[64] = {
  33, 0, 33, 0, 0, 33, 0, 33, // Sourcils agités (pleurs)
  32, 32, 32, 32, 32, 32, 32, 32, // Sourcil continu épais
  0, 23, 21, 22, 22, 21, 23, 0, // Paupière supérieure très tombante
  23, 21, 20, 11, 11, 20, 21, 23,
  22, 20, 10, 31, 31, 10, 20, 22, // Iris encore plus rétrécie
  21, 20, 30, 31, 31, 30, 20, 21,
  32, 32, 32, 32, 32, 32, 32, 32, // Paupière basse tremblante (pleurs)
  0, 33, 23, 33, 33, 23, 33, 0   // Larmes suggérées (coins inférieur)
};

// --- PATTERN #10: ANGRY ----------------------------------------------------
static const uint8_t PATTERN_ANGRY_FRAME0[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 33, 23, 23, 33, 0, 0,
  0, 0, 21, 12, 12, 21, 33, 0,
  22, 21, 31, 30, 30, 31, 21, 22,
  21, 20, 31, 30, 30, 31, 20, 21,
  22, 20, 11, 31, 31, 11, 20, 22,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};
static const uint8_t PATTERN_ANGRY_FRAME1[64] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 33, 23, 23, 33, 0, 0,
  0, 0, 21, 12, 12, 21, 33, 0,
  22, 21, 31, 30, 30, 31, 21, 22,
  21, 20, 31, 30, 30, 31, 20, 21,
  0, 22, 21, 20, 20, 21, 22, 0,
  0, 0, 22, 21, 21, 22, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// --- PATTERN #11: SURPRISED ------------------------------------------------
// Very wide open, small iris
static const uint8_t PATTERN_SURPRISED_FRAME0[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 21, 12, 11, 11, 12, 21, 23,
  22, 20, 10, 11, 11, 10, 20, 22, // Small iris
  22, 20, 10, 11, 11, 10, 20, 22,
  23, 21, 12, 11, 11, 12, 21, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
};
// Pulse pupil size
static const uint8_t PATTERN_SURPRISED_FRAME1[64] = {
  0, 33, 23, 22, 22, 23, 33, 0,
  33, 22, 21, 21, 21, 21, 22, 33,
  23, 21, 12, 11, 11, 12, 21, 23,
  22, 20, 10, 30, 30, 10, 20, 22, // Very small pupil
  22, 20, 10, 30, 30, 10, 20, 22,
  23, 21, 12, 11, 11, 12, 21, 23,
  33, 22, 21, 21, 21, 21, 22, 33,
  0, 33, 23, 22, 22, 23, 33, 0
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

static void drawPattern(Adafruit_NeoPixel& eye, const uint8_t* pattern, uint32_t primary, uint32_t secondary, uint32_t tertiary) {
  for (uint8_t y = 0; y < NEOPIXEL_MATRIX_HEIGHT; y++) {
    for (uint8_t x = 0; x < NEOPIXEL_MATRIX_WIDTH; x++) {
      uint8_t idx = xyToIndex(x, y);
      uint8_t code = pattern[y * NEOPIXEL_MATRIX_WIDTH + x];
      uint32_t finalColor = 0;
      
      if (code == 0) {
        finalColor = 0;
      }
      else if (code >= 10 && code < 20) {
        if (code == 10) finalColor = primary;
        else if (code == 11) finalColor = dimColor(primary, 70);
        else if (code == 12) finalColor = dimColor(primary, 40);
        else finalColor = dimColor(primary, 20);
      }
      else if (code >= 20 && code < 30) {
        if (code == 20) finalColor = secondary;
        else if (code == 21) finalColor = dimColor(secondary, 70);
        else if (code == 22) finalColor = dimColor(secondary, 40);
        else if (code == 23) finalColor = dimColor(secondary, 20);
        else finalColor = dimColor(secondary, 10);
      }
      else if (code >= 30 && code < 40) {
        if (code == 30) finalColor = tertiary;
        else if (code == 31) finalColor = dimColor(tertiary, 70);
        else if (code == 32) finalColor = dimColor(tertiary, 40);
        else finalColor = dimColor(tertiary, 20);
      }
      else if (code == 1) finalColor = primary;
      else if (code == 2) finalColor = secondary;
      else if (code == 3) finalColor = tertiary;
      else if (code == 100) finalColor = primary; // Support for turn signal intensity
      
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

void setPrimaryColor(uint32_t color) { g_eyeState.primaryColor = color; saveSettings(); update(); }
void setSecondaryColor(uint32_t color) { g_eyeState.secondaryColor = color; saveSettings(); update(); }
void setTertiaryColor(uint32_t color) { g_eyeState.tertiaryColor = color; saveSettings(); update(); }

void applyColorScheme(ColorScheme scheme) {
  g_currentScheme = scheme;
  switch (scheme) {
    case ColorScheme::CARS_ORANGE:
      g_eyeState.primaryColor = SCHEME_CARS_ORANGE[0];
      g_eyeState.secondaryColor = SCHEME_CARS_ORANGE[1];
      g_eyeState.tertiaryColor = SCHEME_CARS_ORANGE[2];
      break;
    case ColorScheme::SOFT_REALISTIC:
      g_eyeState.primaryColor = SCHEME_SOFT_REALISTIC[0];
      g_eyeState.secondaryColor = SCHEME_SOFT_REALISTIC[1];
      g_eyeState.tertiaryColor = SCHEME_SOFT_REALISTIC[2];
      break;
    case ColorScheme::ELEGANT_BLUE:
      g_eyeState.primaryColor = SCHEME_ELEGANT_BLUE[0];
      g_eyeState.secondaryColor = SCHEME_ELEGANT_BLUE[1];
      g_eyeState.tertiaryColor = SCHEME_ELEGANT_BLUE[2];
      break;
  }
  saveSettings();
  update();
}
ColorScheme getCurrentColorScheme() { return g_currentScheme; }

void setAutoPlay(bool enabled) {
  g_eyeState.autoPlay = enabled;
  if(enabled) g_eyeState.lastAnimationChange = millis();
}
bool isAutoPlayEnabled() { return g_eyeState.autoPlay; }

void update() {
  const unsigned long now = millis();
  
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
       // Turn signals use classic AMBER color (0xFFBF00) usually, but we use primary color if custom? 
       // Requirement says "arrow", assuming standard color scheme or specific orange.
       // Let's use standard amber for implementation to look like a car.
       uint32_t cPri = g_eyeState.primaryColor;
       if (anim == AnimationType::TURN_LEFT || anim == AnimationType::TURN_RIGHT || anim == AnimationType::HAZARD) {
           cPri = 0x00FFBF00; // Amber
       }
       drawPattern(eyeLeft, patternLeft, cPri, g_eyeState.secondaryColor, g_eyeState.tertiaryColor);
    } else {
       eyeLeft.clear();
    }
    
    // Draw Right
    if (patternRight) {
       uint32_t cPri = g_eyeState.primaryColor;
       if (anim == AnimationType::TURN_LEFT || anim == AnimationType::TURN_RIGHT || anim == AnimationType::HAZARD) {
           cPri = 0x00FFBF00; // Amber
       }
       drawPattern(eyeRight, patternRight, cPri, g_eyeState.secondaryColor, g_eyeState.tertiaryColor);
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
