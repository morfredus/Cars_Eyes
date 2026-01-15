# User Guide

**Version:** 1.5.22  
**Language:** English  
**Minimum version:** 1.0.0  

## Configure
- Secrets: copy `include/secrets_example.h` to `include/secrets.h` and list SSID/password pairs.
- User options: edit `include/config.h` (mDNS host, LCD rotation, WiFi timeouts, NeoPixel toggle).
- Pin map: review `include/board_config.h` for each environment before wiring.

## Environments
- `esp32devkit`: classic ESP32 DevKit, no NeoPixel by default, LCD over VSPI (SCK 18, MOSI 23, CS 15, DC 16, RST 14, BL 27), buttons on 0/32/33.
- `esp32s3_n16r8`: ESP32-S3 DevKitC-1 N16R8, NeoPixel on GPIO48, LCD SPI on 12/11/13 with CS 10, DC 9, RST 8, BL 7, buttons on 0/16/17.

## Add modules
- Place common code in `src/` or `lib/`. Keep headers in `include/`.
- Add new libraries via `lib_deps` in `[env]` to avoid duplication across environments.
- When adding sensors, extend `board_config.h` with dedicated pins and document them in `docs` and README.

## Modify pin mapping
- `include/board_config.h` is the single source of truth. Update pins per environment, keep BOOT on GPIO0 and NeoPixel on GPIO48 for S3.
- Avoid S3 strapping pins 45/46 and ESP32 strapping pins 0/2/4/5/12/15 for outputs unless required.
- After changes, sync the documentation and wiring diagrams.

## Physical Controls (Buttons)
The system supports 3 physical buttons for direct control:
- **Button 1 (GPIO 0 BOOT)**: Hold during boot for Firmware Mode. In App: Short press/hold for **Left Turn Signal**.
- **Button 2 (User 1)**: Short press/hold for **Left Turn Signal** (Primary mapping).
- **Button 3 (User 2)**: Short press/hold for **Right Turn Signal**.

**Signal Logic:**
- **Short Press (< 0.5s)**: Activates signal for configured duration (Default 3s).
- **Long Press (> 0.5s)**: Activates signal indefinitely until pressed again.
- **Hazard Mode**: Press **Button 2 + Button 3** simultaneously to toggle Hazard lights (Both arrows).
- While signaling, the opposite eye remains in "Idle" mode (open and live).

## WiFi behavior
- Uses `WiFiMulti`: all networks from `secrets.h` are tried in order.
- LCD boot screen shows progress; on success, SSID and IP appear on the main screen.
- mDNS hostname comes from `config.h` and is registered on port 80.

## LCD system
- ST7789 over SPI with Adafruit GFX. No full-screen clear loops at runtime; updates are targeted to limit flicker.
- Boot bar is enabled by default; disable via `enableBootBar` if needed.
- Backlight uses digital control (HIGH/LOW); adjust behavior in `tft_display.cpp`.

## Web UI
- Served by the built-in `WebServer` on port 80.
- Access via IP or `http://<mdns>.local/` when mDNS is active.
- Cards display CPU type, speed, heap/PSRAM usage, SSID, and IP. JSON status is available at `/status`.

## Project structure
- `platformio.ini`: environments, shared build flags, dependencies, project metadata.
- `include/`: configuration (`config.h`, `board_config.h`, secrets) and shared headers.
- `src/`: firmware entry point (`main.cpp`) and features.
- `docs/`: bilingual user documentation.
- `test/`, `lib/`: placeholders for unit tests and local libraries.

## Best practices
- Keep project name/version only in `platformio.ini`; use macros in code.
- Prefer platform libraries first; add third-party deps sparingly in `[env]`.
- Test both environments after changes; watch GPIO safety notes above.
- Commit updates with matching README/CHANGELOG updates for every version bump.

## Physical Controls (Buttons) - Extended
The system supports additional physical controls for enhanced signaling:
- **Button 4 (User 3)**: Short press/hold for **Warning Signal** (flashes both left and right signals).
- **Button 5 (User 4)**: Short press/hold for **Panic Signal** (flashes signals and turns on headlights).

**Extended Signal Logic:**
- **Warning Signal**: Activates both left and right turn signals alternately.
- **Panic Signal**: Activates all signals and headlights; press again to cancel.

## Note
- Ensure all buttons are correctly wired and configured in `board_config.h`.
- Test each button's functionality after setup to confirm correct operation.




