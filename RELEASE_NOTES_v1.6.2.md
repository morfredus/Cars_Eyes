# Release Notes v1.6.2 — 2026-01-17

**Status:** ✅ RELEASED  
**Branch:** `main` (merged from `sauv/v1.6.0`)  
**Tag:** `v1.6.2`  
**Type:** PATCH (SEMVER)

---

## 📋 Executive Summary

v1.6.2 completes the enhanced eye animation system with critical fixes and standardization:
- ✅ OFF button now works persistently (stops all animations)
- ✅ Directional gaze animations (LOOK_UP/DOWN) standardized via matrix rotation
- ✅ LED control optimized with correct GPIO pins for RMT support
- ✅ All display outputs corrected (LCD, Web UI)

---

## 🎯 Key Features & Fixes

### CHANGE #6: OFF Button Persistence Fix
**File:** `src/web_server.cpp`  
**Issue:** OFF button didn't keep LEDs off; animations would restart.  
**Fix:** Modified `/api/eyes/off` endpoint to call `setAutoPlay(false)` before clearing LEDs.  
**Status:** ✅ Completed in v1.6.1, improved in v1.6.2

### CHANGE #7: LOOK_LEFT & LOOK_RIGHT Iris Codes Correction
**Files:** `src/neopixel.cpp`, `docs/NEOPIXEL_PATTERNS*.md`  
**Issue:** Iris codes were 80/81 (blinking pupils) instead of proper core structure.  
**Fix:** Changed iris codes from 80/81 to 10/11 for consistent pupil rendering across all directional patterns.  
**Details:**
- Frame0: Pupils 10/11/80 codes
- Frame1: Pupils 81/80 codes (alternating blink)

### CHANGE #8: LOOK_UP & LOOK_DOWN Matrix Rotation Method
**Files:** `src/neopixel.cpp`, `docs/NEOPIXEL_PATTERNS*.md`  
**Method:** Generated via 90° rotation of LOOK_LEFT pattern
- **LOOK_UP:** LOOK_LEFT rotated 90° clockwise → iris positioned at top (rows 1-3)
- **LOOK_DOWN:** LOOK_LEFT rotated 90° counter-clockwise → iris positioned at bottom (rows 4-6)
- **Result:** Consistent structure with all directional patterns; proper vertical/horizontal iris positioning

### CHANGE #9: OFF Button Complete Fix - turnOff() Function
**Files:** `include/neopixel.h`, `src/neopixel.cpp`, `src/web_server.cpp`  
**Problem:** After changes #6 (GPIO fix), OFF button still didn't persist—`update()` would restart IDLE animation.  
**Solution Implemented:**
1. Added `AnimationType::OFF` special state (index 17)
2. New `turnOff()` function:
   - Disables autoplay
   - Clears signal state
   - Forces animation state to OFF
   - Clears & shows LEDs
3. Modified `update()` to skip all logic when animation is OFF
4. Endpoint `/api/eyes/off` now calls `turnOff()` instead of manual clear

### CHANGE #10: LCD Display Fix for OFF State
**File:** `src/tft_display.cpp`  
**Issue:** LCD showed "UNKNOWN" or corrupted text when OFF button was pressed.  
**Fix:**
- Added "OFF" as 18th element in `animNames[]` array
- Updated bounds check from `< 17` to `< 18`
- Result: LCD now displays "OFF" correctly

---

## 🔧 Technical Details

### Animation Pattern Updates
All four directional animations now use standardized structure:
```
STRUCTURE: Full 8-row open eyes with directional iris positioning + blinking pupils
PUPILS: Codes 10/11 (core) alternate with 80/81 (blink) between Frame0/Frame1
```

### GPIO Configuration (v1.6.2 Final)
```
NEOPIXEL_EYE_LEFT_PIN:   GPIO 3  (RMT0 optimal support)
NEOPIXEL_EYE_RIGHT_PIN:  GPIO 6  (RMT3 optimal support)
NEOPIXEL_STATUS_PIN:     GPIO 47 (RMT support)
```
**History:** 1→2 (UART conflict) → 41→42 (suboptimal) → 3→6→47 (RMT-optimal)

### Palette System (v1.6.0 / Inherited)
- **CARS_ORANGE_V2:** 0x00FF5500, 0x00FFAA55, 0x00662200, 0x00FFF2E5, 0x00E0F4FF, 0x000A1A33, 0x00CC4400, 0x00000000
- **HUMAN_EYE:** 0x00C48A4A, 0x008B5A2B, 0x00402010, 0x006A7F4F, 0x00F7F2E8, 0x00E6F7FF, 0x00445E80, 0x00000000
- **Perceived Brightness:** When UI brightness < 64, extra dimming applied to very bright colors

---

## 📊 Commits & Changes Summary

| # | Change | Files Modified | Status |
|---|--------|-----------------|--------|
| 6 | OFF button persistence | `src/web_server.cpp` | ✅ v1.6.1 → improved v1.6.2 |
| 7 | LOOK_LEFT/RIGHT iris codes | `src/neopixel.cpp`, `docs/NEOPIXEL_PATTERNS*.md` | ✅ v1.6.2 |
| 8 | LOOK_UP/DOWN matrix rotation | `src/neopixel.cpp`, `docs/NEOPIXEL_PATTERNS*.md` | ✅ v1.6.2 |
| 9 | OFF button turnOff() function | `include/neopixel.h`, `src/neopixel.cpp`, `src/web_server.cpp` | ✅ v1.6.2 |
| 10 | LCD OFF state display | `src/tft_display.cpp`, `CHANGELOG*.md` | ✅ v1.6.2 |

**Total Commits:** 11 (including documentation updates & merges)

---

## 📚 Documentation Updates

### Files Updated (EN & FR):
- ✅ `CHANGELOG.md` / `CHANGELOG_FR.md` — All 5 changes documented
- ✅ `docs/NEOPIXEL_PATTERNS.md` / `docs/NEOPIXEL_PATTERNS_FR.md` — Pattern grids updated with current data
- ✅ `docs/WEB_UI.md` / `docs/WEB_UI_FR.md` — `/api/eyes/off` endpoint description refined
- ✅ `README.md` / `README_FR.md` — Inherited from v1.6.0

### Files Deleted (Obsolete):
- ❌ `docs/BLINK_ANIMATION.md/FR.md` — Functionality moved to NEOPIXEL_PATTERNS
- ❌ `docs/INDEX_PATTERNS_NEOPIXEL.md/FR.md` — Redundant with NEOPIXEL_PATTERNS
- ❌ `docs/LCD_SYSTEM.md/FR.md` — Information in board_config.h & README

---

## ✅ Testing & Validation

- ✅ Code compiles without errors/warnings
- ✅ Firmware successfully uploaded (865,456 bytes)
- ✅ Build time: ~10.16 sec
- ✅ Upload time: ~21.84 sec
- ✅ LCD display test: "OFF" label renders correctly
- ✅ Web UI: OFF button functional and persistent
- ✅ GPIO pins verified for RMT support
- ✅ Animation patterns verified against neopixel.cpp source

---

## 🔄 Migration Notes

### From v1.6.1 → v1.6.2
- No breaking changes
- OFF button now requires single click (not multiple)
- LCD display now shows correct state labels
- Animation quality improved with corrected iris codes

### Configuration (No changes needed)
- platformio.ini: version bumped from "1.6.1" to "1.6.2"
- No new config entries
- GPIO mappings already optimized

---

## 📦 Installation

### PlatformIO
```bash
git clone https://github.com/morfredus/Cars_Eyes.git
cd Cars_Eyes
git checkout v1.6.2
pio run --target upload
```

### OTA Update
1. Navigate to `http://cars-eyes.local/update` (or your device IP)
2. Upload compiled `.bin` file from release assets
3. Device reboots automatically

---

## 🐛 Known Issues & Limitations

- None reported in v1.6.2
- Previous GPIO conflicts resolved
- Previous animation rendering issues resolved

---

## 🚀 Future Roadmap (Post v1.6.2)

- v1.7.0: New animation patterns (WINK, TWITCH, FOCUS)
- v1.8.0: Sequence/combo system (multi-animation playback)
- v2.0.0: Multi-device networking support

---

## 👨‍💻 Contributors & Acknowledgments

- **morfredus** — Lead development, testing, documentation

---

## 📝 License

See [licence](licence) file in repository root.

---

**Release Date:** January 17, 2026  
**Verified:** ✅ January 17, 2026  
**Status:** 🟢 STABLE
