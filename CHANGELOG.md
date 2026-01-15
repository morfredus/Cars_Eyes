# Changelog

All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## [1.5.6] - 2026-01-15

### Fixed
- **Compilation**: Fixed a variable redeclaration error in `tft_display.cpp` which caused the build to fail in version 1.5.5.

## [1.5.5] - 2026-01-15

### Fixed
- **LCD Display**: Fixed a bug where the "MODE" status on the LCD screen would not update when the animation was changed via the Web UI.

## [1.5.4] - 2026-01-15

### Changed
- **Turn Signal Design**: Updated arrow patterns to use the full width of the matrix (8 pixels), simulating a more realistic automotive arrow style.

## [1.5.3] - 2026-01-15

### Fixed
- **Rendering**: Fixed rendering engine to correctly interpret the specialized pixel code (100) used for Turn Signal patterns, which was previously resulting in blank output.

## [1.5.2] - 2026-01-15

### Fixed
- **Button Logic**: Completely rewrote button handling logic to fix unresponsiveness on Button 1 and improve press detection.
- **Turn Signal Visuals**: The "non-signaling" eye now remains open (Idle pattern) instead of turning off while the other signal is active.
- **Hazard Mode**: Corrected detection for simultaneous button presses (Warning mode).

## [1.5.1] - 2026-01-15

### Fixed
- **Boot Button Logic**: Corrected an inversion in the boot button detection logic introduced in 1.5.0 which caused a reboot prompt loop.

## [1.5.0] - 2026-01-15

### Added
- **Turn Signals**: New feature triggered by physical buttons.
  - Button 1: Left Turn Signal (Arrow Left).
  - Button 2: Right Turn Signal (Arrow Right).
  - Button 1+2: Warning/Hazard Mode (Both Arrows).
  - Short Press: Active for configurable duration.
  - Long Press: Active indefinitely until cancelled.
- **Persistence**: Application settings are now saved to NVS memory and restored on boot.
  - Saves: Brightness, Selected Colors, Animation Mode, Color Scheme, Signal Duration.
- **Web UI Configuration**: Added check slider to configure Turn Signal duration (default 3000ms).

## [1.4.2] - 2026-01-15

### Fixed
- **Main Screen Layout**: Moved the separator line and "MODE" status down to prevent overlapping with the mDNS address line.

## [1.4.1] - 2026-01-15

### Changed
- **IDLE Animation**: Added random blinking behavior to IDLE state (every 3-8 seconds) for a more lifelike appearance.
- **BLINK Animation**: Logic updated to always return to IDLE state after completion, preventing infinite blinking loops.

## [1.4.0] - 2026-01-15

### Added
- **Cars Theme UI**: Completely redesigned Boot Screen and Main Screen with "Cars" movie aesthetics.
- **Boot Screen**: Added animated Lightning McQueen style lightning bolt and red background.
- **Main Screen**: Added checkerboard pattern, better layout, and visual WiFi status.
- **Progress Bar**: Improved WiFi connection progress bar visual style.

## [1.3.2] - 2026-01-15

### Fixed
- **Animation Glitch**: Fixed animation freeze when manually changing effects by prioritizing user input over AutoPlay timer.
- **LOOK_UP Animation**: Corrected `LOOK_UP` pattern to exactly mirror `LOOK_DOWN` logic (iris moving up) for consistency.
- **Latency**: Removed frame delay on effect switch for instant response.

## [1.3.1] - 2026-01-15

### Fixed
- **LCD Display**: Fixed black screen issue on ESP32-S3 by switching backlight control from PWM to digital HIGH.
- **Neopixel Animations**: Implemented missing SAD, SLEEP, and SURPRISED patterns.
- **Performance**: Fixed animation freezes in AutoPlay loop.

### Changed
- **IDLE Animation**: Made idle state dynamic with subtle eyelid movements.
- **Look Logic**: Improved LOOK_UP/LOOK_DOWN to be mirrored and realistic.

## [1.3.0] - 2026-01-15

### Added
- **Rendering Engine**: Complete rewrite of `neopixel.cpp` to support anti-aliasing (partial brightness).
- **Color Schemes**: Added "Cars" (Lightning McQueen), "Realistic", and "Elegant" visual themes.

## [1.1.0] - 2026-01-14

### Change 5 - Enhancement: More expressive BLINK pattern
**File:** `src/neopixel.cpp` (lines 47-56)  
**Problem:** The original BLINK pattern used a simple flat horizontal line (rows 3-4 only), which was not expressive enough to simulate a natural eye blink.  
**Solution:** Complete pattern redesign to create a rounded closed-eye shape spanning 6 active rows (rows 1-6). The pattern now shows upper and lower eyelids converging toward a central closure point.  
**Impact:** Much more natural and expressive blink animation, enhancing the personality of the animated eyes.

### Change 4 - Fix: xyToIndex() function for linear LED numbering
**File:** `src/neopixel.cpp` (lines 155-164)  
**Problem:** The xyToIndex() function implemented zigzag compensation (mirroring odd rows), but user revealed that LED matrices are wired linearly (0-63 in reading order).  
**Solution:** Complete simplification of xyToIndex() to return `y * NEOPIXEL_MATRIX_WIDTH + x`, removing all zigzag logic.  
**Impact:** All patterns now display correctly without even/odd row misalignment.

### Change 3 - Fix: LOOK_LEFT and LOOK_RIGHT patterns
**File:** `src/neopixel.cpp` (lines 60-81)  
**Problem:** LOOK_LEFT and LOOK_RIGHT patterns didn't follow linear LED numbering and didn't move the entire eye like LOOK_UP/LOOK_DOWN did.  
**Solution:** Complete redesign of both patterns to move a full rounded eye (like LOOK_UP/DOWN). LOOK_LEFT positions eye at columns 1-4, LOOK_RIGHT at columns 3-6. Patterns designed for linear numbering.  
**Impact:** Eyes now move correctly left and right with smooth, symmetric motion.

### Change 2 - Enhancement: Web UI text contrast
**File:** `src/web_ui.cpp` (CSS styles, lines ~20-55)  
**Problem:** System information texts (Brightness: 32, Auto-Play Mode, ESP32-S3, 240 MHz, etc.) were not readable due to insufficient contrast (default gray text on white background).  
**Solution:** CSS enhancement with darker colors (#222 for .card div, #333 for labels) and added font-weight (500-600) for key elements (.card strong, .color-group label, .slider-label, button).  
**Impact:** All web interface texts are now clearly readable.

### Change 1 - Fix: Obsolete setColor() function
**File:** `src/buttons.cpp` (line 41)  
**Problem:** Code was calling `NeoPixel::setColor()` which no longer exists in the refactored NeoPixel module API.  
**Solution:** Replaced with `NeoPixel::setStatusLed(NeoPixel::makeColor(0, 0, 255))` to set status LED to blue before restart.  
**Impact:** Fixed compilation error, system now compiles successfully.

---

## [1.0.0] - 2026-01-13

### NeoPixel Eyes Project Creation
First stable release of animated eyes controller with 8x8 NeoPixel matrices.

### Supported Hardware
- **ESP32-S3 DevKitC-1 N16R8** (16MB Flash, 8MB PSRAM)
- **2x NeoPixel 8x8 Matrices** (128 WS2812B LEDs total)
- **ST7789 TFT Display** (240x240 pixels)
- **Status LED** (GPIO48)
- **Physical Buttons** (reset/reboot)

### Core Features

#### 1. Animation System
- 13 animation types: IDLE, BLINK, WINK_LEFT, WINK_RIGHT, LOOK_LEFT, LOOK_RIGHT, LOOK_UP, LOOK_DOWN, HAPPY, SAD, ANGRY, SURPRISED, SLEEP
- Pattern-based 8x8 system (0=off, 1=primary color, 2=secondary color)
- Auto-play mode with random animations every 3-5 seconds
- Brightness control (0-255)
- Primary (outline) and secondary (fill) color configuration

#### 2. Modern Web Interface
- Purple gradient design (#667eea to #764ba2)
- Real-time animation control
- Brightness adjustment with slider
- Primary/secondary color selection
- System information display (ESP32-S3, CPU, RAM, Flash, IP, SSID)
- REST API with 5 endpoints:
  - `/api/eyes/status` - System state
  - `/api/eyes/animation` - Animation control
  - `/api/eyes/brightness` - Brightness adjustment
  - `/api/eyes/color` - Color configuration
  - `/api/eyes/autoplay` - Auto-play mode
- Real-time updates via fetch API (2-second interval)

#### 3. ST7789 TFT Display
- Network information display (IP, SSID, mDNS hostname)
- Dynamic WiFi and mDNS status
- System information (version, CPU frequency, RAM)
- Visual monitoring interface

#### 4. Network Connectivity
- WiFiMulti with multi-network support
- mDNS service (local network discovery)
- OTA (Over-The-Air updates)
- Robust disconnection handling

#### 5. Modular Architecture
Code separated into independent modules with namespaces:
- `tft_display.h/cpp` - ST7789 display and rendering
- `web_ui.h/cpp` - HTML page and JSON builders
- `web_styles.h/cpp` - CSS styles and JavaScript
- `web_server.h/cpp` - HTTP server and routes
- `network.h/cpp` - WiFi, mDNS, OTA
- `neopixel.h/cpp` - NeoPixel LED management
- `buttons.h/cpp` - Button input and reboot
- `ui_state.h/cpp` - Global interface state
- `psram.h/cpp` - PSRAM initialization

#### 6. GPIO Configuration (board_config.h)
- **GPIO4** - NeoPixel left eye (64 LEDs)
- **GPIO5** - NeoPixel right eye (64 LEDs)
- **GPIO48** - Status LED
- **GPIO6** - TFT CS (Chip Select)
- **GPIO7** - TFT DC (Data/Command)
- **GPIO17** - TFT RST (Reset)
- **GPIO18** - TFT SPI SCK (Clock)
- **GPIO16** - TFT SPI MOSI (Data)
- **GPIO0** - BOOT Button

#### 7. Security and Configuration
- Security wrapper for WiFi credentials (`secrets_wrapper.h/cpp`)
- Isolated secrets.h file (single inclusion point)
- Centralized configuration in `config.h`
- Index validation to prevent buffer overflow

#### 8. Complete Documentation
- Bilingual documentation (EN/FR) in `/docs`
- Getting started guide (GETTING_STARTED)
- Modular architecture (MODULAR_ARCHITECTURE)
- Project structure (PROJECT_STRUCTURE)
- Pin mapping (PIN_MAPPING)
- LCD system (LCD_SYSTEM)
- Web interface (WEB_UI)
- User guide (USER_GUIDE)
- Adding modules (ADD_MODULES)
- Module dependencies (MODULE_DEPENDENCIES)

### Technical Specifications
- **Platform:** PlatformIO
- **Framework:** Arduino
- **C++ Standard:** C++17
- **CPU Frequency:** 240 MHz
- **Partition:** 16MB Flash (3MB APP, 9MB FATFS, 3MB OTA)
- **PSRAM:** 8MB OPI (Octal SPI)
- **Libraries:**
  - Adafruit NeoPixel v1.12.0
  - Adafruit GFX Library v1.11.11
  - Adafruit ST7789 Library v1.10.4
  - WiFi, ESPmDNS, WebServer, ArduinoOTA (ESP32 core)

### Build and Compilation
- **Environment:** `esp32s3_n16r8`
- **Build Directory:** `C:/pio_builds/build`
- **Compilation Time:** ~10 seconds
- **RAM Usage:** 15.2% (49872/327680 bytes)
- **Flash Usage:** 25.1% (838273/3342336 bytes)

### Default Configuration
- **Brightness:** 32/255
- **Primary Color:** 0xFF4500 (orange)
- **Secondary Color:** 0xFFFFFF (white)
- **Initial Animation:** IDLE
- **Auto-play:** Disabled by default
- **mDNS Hostname:** eyes-controller.local
