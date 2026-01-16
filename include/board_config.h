#pragma once
#include <Arduino.h>

// ╔══════════════════════════════════════════════════════════════════════════════╗
// ║                        GUIDE DEBUTANT - BOARD_CONFIG.H                       ║
// ╠══════════════════════════════════════════════════════════════════════════════╣
// ║  Ce fichier définit le "câblage" entre l'ESP32 et les périphériques.         ║
// ║                                                                              ║
// ║  VOCABULAIRE ESSENTIEL :                                                     ║
// ║  GPIO  = General Purpose Input/Output (broche programmable)                  ║
// ║  PIN   = Broche physique sur la carte                                        ║
// ║  3.3V  = Tension logique ESP32 (JAMAIS dépasser 3.3V sur une GPIO !)         ║
// ║  GND   = Masse électrique (0V, référence commune)                            ║
// ║  VCC   = Alimentation positive (3.3V ou 5V selon composant)                  ║
// ║                                                                              ║
// ║  RÈGLES DE SÉCURITÉ :                                                        ║
// ║  - Ne JAMAIS connecter plus de 3.3V sur une GPIO ESP32                       ║
// ║  - Ne JAMAIS tirer plus de 12mA par GPIO (40mA max absolu)                   ║
// ║  - Toujours connecter GND entre ESP32 et périphériques                       ║
// ║  - Utiliser résistances de protection si nécessaire                          ║
// ╚══════════════════════════════════════════════════════════════════════════════╝

// ============================================================================
// CONFIGURATION ESP32-S3 DevKitC-1 N16R8
// ============================================================================
#if defined(ENV_ESP32S3_N16R8)

// ────────────────────────────────────────────────────────────────────────────
// BOUTONS POUSSOIRS
// ────────────────────────────────────────────────────────────────────────────
// CÂBLAGE : GPIO ──┤├── GND (bouton NO + INPUT_PULLUP)
// Bouton NON appuyé = HIGH (3.3V), Bouton appuyé = LOW (0V)
// Composants : Bouton NO, résistance 10kΩ (optionnel), condensateur 100nF
// ────────────────────────────────────────────────────────────────────────────

#define BUTTON_BOOT_PIN     0   // Bouton BOOT intégré (RTC_GPIO0)
                                // Usage : Appui long 3s = redémarrage
                                // Note : Utilisé pour flasher firmware

#define BUTTON_USER1_PIN    16  // Bouton utilisateur 1 (U0RTS)
                                // Libre d'usage, configurer en INPUT_PULLUP

#define BUTTON_USER2_PIN    17  // Bouton utilisateur 2 (U0CTS)
                                // Libre d'usage, configurer en INPUT_PULLUP

// ────────────────────────────────────────────────────────────────────────────
// ÉCRAN LCD ST7789 (Interface SPI)
// ────────────────────────────────────────────────────────────────────────────
// SPI = Serial Peripheral Interface (bus série synchrone rapide)
// Signaux : SCK (horloge), MOSI (données →), MISO (données ←), CS (sélection)
//
// CÂBLAGE :
//   ESP32-S3      →  Module ST7789
//   3.3V          →  VCC
//   GND           →  GND
//   GPIO12 (SCK)  →  SCK/CLK/SCLK
//   GPIO11 (MOSI) →  SDA/DIN/MOSI  ⚠️ "SDA" sur écran ≠ SDA I2C !
//   GPIO10 (CS)   →  CS/CE/SS
//   GPIO9  (DC)   →  DC/RS/A0
//   GPIO8  (RST)  →  RST/RES/RESET
//   GPIO7  (BL)   →  BL/LED (via transistor si >20mA)
//
// Composants : Condensateurs 100nF + 10µF (filtrage VCC-GND)
// Tension : 3.3V (logique et alimentation)
// ────────────────────────────────────────────────────────────────────────────

#define DISPLAY_SCK_PIN     12  // Horloge SPI (HSPI_CLK, SPI2_CLK)
#define DISPLAY_MOSI_PIN    11  // Données vers écran (HSPI_MOSI, SPI2_MOSI)
#define DISPLAY_MISO_PIN    13  // Données depuis écran (HSPI_MISO, optionnel)
#define DISPLAY_CS_PIN      10  // Chip Select (actif LOW)
#define DISPLAY_DC_PIN      9   // Data/Command (LOW=cmd, HIGH=data)
#define DISPLAY_RST_PIN     8   // Reset (pulse LOW pour réinitialiser)
#define DISPLAY_BL_PIN      7   // Backlight PWM (0=éteint, 255=max)

#define DISPLAY_WIDTH       240   // Largeur écran en pixels
#define DISPLAY_HEIGHT      320   // Hauteur écran en pixels
#define DISPLAY_SPI_FREQ    40000000UL  // Fréquence SPI : 40 MHz

// ────────────────────────────────────────────────────────────────────────────
// LED RGB NEOPIXEL (WS2812B) - MATRICES 8x8
// ────────────────────────────────────────────────────────────────────────────
// LED RGB adressable avec puce WS2812B intégrée
// Un seul fil de données contrôle des centaines de LEDs en chaîne
//
// CÂBLAGE :
//   5V           →  VCC NeoPixel + condensateur 100µF
//   GND          →  GND NeoPixel
//   GPIO41       →  DIN Œil Gauche (via résistance 330Ω protection)
//   GPIO42       →  DIN Œil Droit (via résistance 330Ω protection)
//   GPIO48       →  DIN LED Status (via résistance 330Ω protection)
//
// ⚠️ GPIO LIBRES RECOMMANDÉES sur ESP32-S3 :
// - GPIO 3-6, 14-21, 33-37, 41-46 (éviter 1-2=UART, 7-13=SPI, 15-17=LCD, 48=LED)
//
// Composants : Résistance 330Ω, condensateur 100µF électrolytique
// Tensions : 5V alimentation LED, 3.3V signal DIN
// Consommation : ~60mA par LED à pleine luminosité blanche
// ────────────────────────────────────────────────────────────────────────────

#define NEOPIXEL_EYE_LEFT_PIN   41  // Broche données matrice œil gauche (GPIO libre)
#define NEOPIXEL_EYE_RIGHT_PIN  42  // Broche données matrice œil droit (GPIO libre)
#define NEOPIXEL_STATUS_PIN     48  // LED RGB intégrée (RTC_GPIO18)

#define NEOPIXEL_MATRIX_WIDTH   8   // Largeur matrice en LEDs
#define NEOPIXEL_MATRIX_HEIGHT  8   // Hauteur matrice en LEDs
#define NEOPIXEL_MATRIX_COUNT   64  // Total LEDs par matrice (8x8)
#define NEOPIXEL_BRIGHTNESS     32  // Luminosité par défaut (0-255)
                                    // Conseil : 32=économie, 128=moyen

// ============================================================================
// CONFIGURATION ESP32 DEVKIT V1 (WROOM-32)
// ============================================================================
#elif defined(ENV_ESP32_DEVKIT)

// ────────────────────────────────────────────────────────────────────────────
// BOUTONS POUSSOIRS
// ────────────────────────────────────────────────────────────────────────────
// CÂBLAGE : GPIO ──┤├── GND (bouton NO + INPUT_PULLUP)
// Bouton NON appuyé = HIGH (3.3V), Bouton appuyé = LOW (0V)
// ────────────────────────────────────────────────────────────────────────────

#define BUTTON_BOOT_PIN     0   // Bouton BOOT intégré (RTC_GPIO0)
                                // Usage : Appui long 3s = redémarrage

#define BUTTON_USER1_PIN    32  // Bouton utilisateur 1 (ADC1_CH4, RTC_GPIO9)
                                // Supporte entrée analogique 0-3.3V

#define BUTTON_USER2_PIN    33  // Bouton utilisateur 2 (ADC1_CH5, RTC_GPIO8)
                                // Supporte entrée analogique 0-3.3V

// ────────────────────────────────────────────────────────────────────────────
// ÉCRAN LCD ST7789 (Interface SPI via VSPI/SPI3)
// ────────────────────────────────────────────────────────────────────────────
// CÂBLAGE :
//   ESP32 DevKit  →  Module ST7789
//   3.3V          →  VCC
//   GND           →  GND
//   GPIO18 (SCK)  →  SCK/CLK/SCLK
//   GPIO23 (MOSI) →  SDA/DIN/MOSI
//   GPIO5  (CS)   →  CS/CE/SS       ✅ MODIFIÉ (était GPIO15)
//   GPIO16 (DC)   →  DC/RS/A0
//   GPIO4  (RST)  →  RST/RES/RESET  ✅ MODIFIÉ (était GPIO14)
//   GPIO27 (BL)   →  BL/LED
//
// ⚠️ CHANGEMENTS SÉCURITÉ :
//   - GPIO15 → GPIO5  (GPIO15 = strapping pin, problème au boot)
//   - GPIO14 → GPIO4  (GPIO14 = JTAG TMS, possible conflit)
//
// Composants : Condensateurs 100nF + 10µF (filtrage VCC-GND)
// Tension : 3.3V (logique et alimentation)
// ────────────────────────────────────────────────────────────────────────────

#define DISPLAY_SCK_PIN     18  // Horloge SPI (VSPI_CLK, SPI3_CLK)
#define DISPLAY_MOSI_PIN    23  // Données vers écran (VSPI_MOSI, SPI3_MOSI)
#define DISPLAY_MISO_PIN    19  // Données depuis écran (VSPI_MISO, optionnel)
#define DISPLAY_CS_PIN      5   // Chip Select (actif LOW)
                                // ✅ SAFE : Évite GPIO15 (strapping)
#define DISPLAY_DC_PIN      16  // Data/Command (LOW=cmd, HIGH=data)
#define DISPLAY_RST_PIN     4   // Reset (pulse LOW pour réinitialiser)
                                // ✅ SAFE : Évite GPIO14 (JTAG)
#define DISPLAY_BL_PIN      27  // Backlight PWM (0=éteint, 255=max)

#define DISPLAY_WIDTH       240   // Largeur écran en pixels
#define DISPLAY_HEIGHT      240   // Hauteur écran en pixels
#define DISPLAY_SPI_FREQ    27000000UL  // Fréquence SPI : 27 MHz

// ────────────────────────────────────────────────────────────────────────────
// NEOPIXEL NON DISPONIBLE
// ────────────────────────────────────────────────────────────────────────────
// L'ESP32 DevKit classique n'a pas de NeoPixel intégré.
// Vous pouvez en ajouter un sur n'importe quelle GPIO (ex: GPIO5 si libre).
// ────────────────────────────────────────────────────────────────────────────

#define NEOPIXEL_AVAILABLE  0   // 0 = NeoPixel non disponible sur cette carte

// ============================================================================
// ERREUR : CARTE NON CONFIGURÉE
// ============================================================================
#else
#error "Board configuration missing. Define ENV_ESP32S3_N16R8 or ENV_ESP32_DEVKIT in build flags."
#endif