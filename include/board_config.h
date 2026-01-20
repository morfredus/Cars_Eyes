/**
 * @file board_config.h
 * @brief Définit le câblage entre l'ESP32 et les périphériques.
 * @note Pour optimiser la mémoire, utilisez des constantes et évitez les variables globales inutiles.
 */

#pragma once
#include <Arduino.h>

// ╔══════════════════════════════════════════════════════════════════════════════╗
// ║                        GUIDE DEBUTANT - BOARD_CONFIG.H                       ║
// ╠══════════════════════════════════════════════════════════════════════════════╣
// ║  Ce fichier definit le "cablage" entre l'ESP32 et les peripheriques.         ║
// ║                                                                              ║
// ║  VOCABULAIRE ESSENTIEL :                                                     ║
// ║  ───────────────────────                                                     ║
// ║  GPIO  = General Purpose Input/Output (broche programmable)                  ║
// ║  PIN   = Broche physique sur la carte                                        ║
// ║  3.3V  = Tension logique de l'ESP32 (JAMAIS depasser 3.3V sur une GPIO !)    ║
// ║  GND   = Masse electrique (0V, reference commune)                            ║
// ║  VCC   = Alimentation positive (3.3V ou 5V selon le composant)               ║
// ║                                                                              ║
// ║  REGLES DE SECURITE :                                                        ║
// ║  ────────────────────                                                        ║
// ║  - Ne JAMAIS connecter plus de 3.3V sur une GPIO de l'ESP32                  ║
// ║  - Ne JAMAIS tirer plus de 12mA par GPIO (40mA max absolu)                   ║
// ║  - Toujours connecter GND entre l'ESP32 et les peripheriques                 ║
// ║  - Utiliser des resistances de protection si necessaire                      ║
// ╚══════════════════════════════════════════════════════════════════════════════╝


// ============================================================================
// CONFIGURATION ESP32-S3 DevKitC-1 N16R8
// ============================================================================
#if defined(ENV_ESP32S3_N16R8)

//
// ────────────────────────────────────────────────────────────────────────────
// BOUTONS POUSSOIRS
// ────────────────────────────────────────────────────────────────────────────
//

#define BUTTON_BOOT_PIN     0   // Bouton BOOT intégré
#define BUTTON_1_PIN        38  // Bouton utilisateur 1 (INPUT_PULLUP)
#define BUTTON_2_PIN        39  // Bouton utilisateur 2 (INPUT_PULLUP)

//
// ────────────────────────────────────────────────────────────────────────────
// ECRAN LCD ST7789 (Interface SPI)
// ────────────────────────────────────────────────────────────────────────────
//

#define DISPLAY_SCK_PIN     12  // SPI SCLK (HSPI natif)
#define DISPLAY_MOSI_PIN    11  // SPI MOSI
#define DISPLAY_MISO_PIN    13  // SPI MISO
#define DISPLAY_CS_PIN      10  // Chip Select TFT
#define DISPLAY_DC_PIN       9  // Data/Command
#define DISPLAY_RST_PIN      8  // Reset écran
#define DISPLAY_BL_PIN      18  // Backlight PWM

#define DISPLAY_WIDTH       240
#define DISPLAY_HEIGHT      320
#define DISPLAY_SPI_FREQ    40000000UL

//
// ────────────────────────────────────────────────────────────────────────────
// MATRICES NEOPIXEL 8x8 (WS2812B)
// ────────────────────────────────────────────────────────────────────────────
//

#define NEOPIXEL_EYE_LEFT_PIN    3   // Matrice gauche (GPIO safe)
#define NEOPIXEL_EYE_RIGHT_PIN   6   // Matrice droite (GPIO safe)

#define NEOPIXEL_MATRIX_WIDTH   8
#define NEOPIXEL_MATRIX_HEIGHT  8
#define NEOPIXEL_MATRIX_COUNT   64
#define NEOPIXEL_BRIGHTNESS     32  // Luminosité par défaut (0-255)
                                    // Conseil : 32=économie, 128=moyen

//
// ────────────────────────────────────────────────────────────────────────────
// LED RGB / LED STATUT
// ────────────────────────────────────────────────────────────────────────────
//

#define NEOPIXEL_STATUS_PIN 48   // LED RGB intégrée ESP32-S3 (soudée sur GPIO48)


// ============================================================================
// CONFIGURATION ESP32 DEVKIT V1 (WROOM-32)
// ============================================================================
#elif defined(ENV_ESP32_DEVKIT)

//
// ────────────────────────────────────────────────────────────────────────────
// BOUTONS
// ────────────────────────────────────────────────────────────────────────────
//

#define BUTTON_BOOT_PIN     0
#define BUTTON_1_PIN        32
#define BUTTON_2_PIN        33

//
// ────────────────────────────────────────────────────────────────────────────
// ECRAN LCD ST7789 (VSPI)
// ────────────────────────────────────────────────────────────────────────────
//

#define DISPLAY_SCK_PIN     18
#define DISPLAY_MOSI_PIN    23
#define DISPLAY_MISO_PIN    19
#define DISPLAY_CS_PIN      15
#define DISPLAY_DC_PIN      16
#define DISPLAY_RST_PIN     14
#define DISPLAY_BL_PIN      27

#define DISPLAY_WIDTH       240
#define DISPLAY_HEIGHT      240
#define DISPLAY_SPI_FREQ    27000000UL
//
// ────────────────────────────────────────────────────────────────────────────
// I2C
// ────────────────────────────────────────────────────────────────────────────
//

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

//
// ────────────────────────────────────────────────────────────────────────────
// LED RGB externes
// ────────────────────────────────────────────────────────────────────────────
//

#define LED_RED_PIN     13
#define LED_GREEN_PIN   26
#define LED_BLUE_PIN    25

//
// ────────────────────────────────────────────────────────────────────────────
// NEOPIXEL (externe uniquement)
// ────────────────────────────────────────────────────────────────────────────
//

#define NEOPIXEL_AVAILABLE 0   // Pas de NeoPixel intégré sur DevKit V1

#else
#error "Board configuration missing. Define ENV_ESP32S3_N16R8 or ENV_ESP32_DEVKIT in build flags."
#endif
