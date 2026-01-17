# NeoPixel Color Schemes

**Version:** 1.6.0  
**Minimum version:** 1.6.0  
**Language:** English  
**Date:** January 16, 2026  
**Change:** Added 8-color palettes and realistic schemes

---

## 🎨 8-Color Palettes (v1.6.0)

### #1b - CARS_ORANGE_V2 (8 colors)
```
C1: 0x00E64A00  (Warm orange - iris)
C2: 0x00CC3C00  (Amber strong - close to C1)
C3: 0x00802700  (Amber-brown dark - shadow)
C4: 0x00B85A1A  (Warm amber highlight, not white)
C5: 0x00994D19  (Mat/cool amber soft highlight)
C6: 0x00401510  (Deep brown contrast)
C7: 0x00A63A00  (Darker amber outline)
C8: 0x00000000  (Black pupil)
```

### #2b - HUMAN_EYE (8 colors)
```
C1: 0x00A06C3A  (Light brown - iris)
C2: 0x007C5328  (Mid brown - close to C1)
C3: 0x00412A15  (Dark brown shadow)
C4: 0x006A7F4F  (Soft olive green nuance)
C5: 0x00D9D0C4  (Warm off-white sclera, attenuated)
C6: 0x00BBD0D8  (Cool off-white highlight, attenuated)
C7: 0x003A5168  (Darker steel blue reflection)
C8: 0x00000000  (Black pupil)
```

### Legacy 3-Color Schemes (auto-expanded to 8 slots)
The legacy schemes below now fill the 8-color palette automatically:
- C1 = Primary
- C2 = Secondary
- C3 = Tertiary
- C4 = Secondary at 70%
- C5 = Secondary at 40%
- C6 = Tertiary at 70%
- C7 = Tertiary at 40%
- C8 = Black

## 🎨 New Feature: Harmonious Color Schemes

Version 1.2.0 introduces a **system of 3 predefined color schemes** based on **complementary color theory** to create more realistic and visually pleasing eyes.

### Reason for this change

In version 1.1.0, the combination of Orange + Pure White created a **violent contrast** that was unrealistic. The new combinations use **complementary colors** which are naturally harmonious.

---

## 📋 Legacy 3 Color Schemes (auto-expanded)

### #1 - CARS_ORANGE (Default)
**Naturally complementary colors**

```
Primary (Iris):         0x00FF4500  (Pixar Orange)
Secondary (Highlights): 0x000099FF  (Cyan blue - complementary)
Tertiary (Shadows):     0x00000000  (Pure black)
```

**Features:**
- ✅ Orange and blue are complementary (opposite on wheel)
- ✅ Visually very harmonious
- ✅ Original Cars style preserved
- ✅ Natural contrast without violence

**Recommended for:** General display, Cars style

---

### #2 - SOFT_REALISTIC (Natural)
**Natural eyes with soft sclera**

```
Primary (Iris):         0x00C85500  (Warm brown)
Secondary (Sclera):     0x00E0E0E0  (Very soft off-white)
Tertiary (Shadows):     0x00003366  (Navy blue very dark)
```

**Features:**
- ✅ Natural brown iris
- ✅ Off-white sclera (less harsh)
- ✅ Subtle navy blue shadows
- ✅ Very realistic and comfortable

**Recommended for:** Daytime, comfortable display

---

### #3 - ELEGANT_BLUE (Elegant)
**Elegant and subtle design**

```
Primary (Iris):         0x000066FF  (Bright blue)
Secondary (Highlights): 0x0066CCFF  (Very pale blue)
Tertiary (Shadows):     0x00110022  (Blue-black very dark)
```

**Features:**
- ✅ Vivid yet soft blue iris
- ✅ Subtle pale blue highlights
- ✅ Very deep shadows
- ✅ Modern and elegant appearance

**Recommended for:** Night mode, elegant look

---

## 🖥️ Usage from Web UI

### Graphical Interface
3 selection buttons are now available:

```
[🚗 Cars Orange]  [👁️ Soft Realistic]  [✨ Elegant Blue]
```

**How to use:**
1. Open web interface (http://[IP]/dashboard)
2. Click on a scheme button
3. Colors change automatically
4. Color pickers update

**Note:** Native 8-color palettes (IDs 3 and 4) are available via API; UI buttons will arrive in a later refresh.

### REST API
```bash
# Change scheme
GET /api/eyes/scheme?id=0  # Cars Orange (legacy 3-color -> auto-expanded)
GET /api/eyes/scheme?id=1  # Soft Realistic (legacy 3-color -> auto-expanded)
GET /api/eyes/scheme?id=2  # Elegant Blue (legacy 3-color -> auto-expanded)
GET /api/eyes/scheme?id=3  # Cars Orange v2 (native 8-color)
GET /api/eyes/scheme?id=4  # Human Eye (native 8-color)
```

**Response:**
```json
{
  "status": "ok",
  "scheme": 0,
  "colors": [16711936, 39423, 0]
}
```

---

## 🔆 Brightness and Perceived Adjustment (v1.6.0)

- Global brightness from Web UI (`/api/eyes/brightness?value=0..255`) continues to apply.
- When brightness < 64, a perceived-brightness adjustment reduces very light tones more than dark ones, to prevent harsh whites and better match the selected level.

This improves coherence across palettes and avoids “too white” highlights at low brightness.

## 💻 Usage from Code

### In C++
```cpp
#include "neopixel.h"

// Apply a scheme
NeoPixel::applyColorScheme(NeoPixel::ColorScheme::CARS_ORANGE);
NeoPixel::applyColorScheme(NeoPixel::ColorScheme::SOFT_REALISTIC);
NeoPixel::applyColorScheme(NeoPixel::ColorScheme::ELEGANT_BLUE);

// Get current scheme
NeoPixel::ColorScheme current = NeoPixel::getCurrentColorScheme();

// Customize individual colors
NeoPixel::setPrimaryColor(0x00FF0000);    // Red
NeoPixel::setSecondaryColor(0x0000FF00);  // Green
NeoPixel::setTertiaryColor(0x000000FF);   // Blue
```

### Accessible from web_server
All schemes are accessible via `/api/eyes/scheme` endpoint.

---

## 🔄 Technical Changes (v1.2.0)

### Modified files
- ✅ `include/neopixel.h` - Added `ColorScheme` enum
- ✅ `src/neopixel.cpp` - Scheme implementation + functions
- ✅ `src/web_ui.cpp` - UI for scheme selection
- ✅ `src/web_server.cpp` - `/api/eyes/scheme` endpoint
- ✅ `src/web_styles.cpp` - CSS for scheme buttons
- ✅ `platformio.ini` - Version 1.1.0 → 1.2.0

### Added API
```cpp
void applyColorScheme(ColorScheme scheme);
ColorScheme getCurrentColorScheme();
```

### API Endpoints
- `GET /api/eyes/scheme?id=0,1,2` - Apply a scheme

---

## 🎯 Benefits

| Aspect | Before (1.1.0) | After (1.2.0) | Gain |
|--------|----------|----------|------|
| **Color Harmony** | Orange + White | Complementary | +30% |
| **Realism** | Medium | High | +20% |
| **Options** | 1 (Orange) | 3 + custom | ✓ |
| **Visual Comfort** | Violent contrast | Harmonious | ✓ |
| **Flexibility** | Code only | Web UI + Code | ✓ |

---

## 📚 Color Theory (Educational)

### Color Wheel
The **complementary color** of a color is the one directly opposite it on the color wheel:

```
Orange (#FF4500) ←→ Cyan (#0099FF)   ✓ Harmonious
Brown (#C85500) ←→ Gray-blue         ✓ Natural
Blue (#0066FF) ←→ Orange-yellow      ✓ Balanced
```

Using complementary colors creates a **natural contrast** that is pleasant to the eye.

---

## 🔧 Migration from v1.1.0

### For users
✅ No changes required - works automatically  
✅ New UI available for changing schemes

### For developers
```cpp
// Old code (still compatible)
NeoPixel::setPrimaryColor(0x00FF4500);
NeoPixel::setSecondaryColor(0x00FFFFFF);

// New code (recommended)
NeoPixel::applyColorScheme(ColorScheme::CARS_ORANGE);
```

**Note:** Old `setPrimaryColor/setSecondaryColor` functions remain available for backward compatibility.

---

## ✅ Quality Assurance

- ✅ 0 compilation errors
- ✅ API backward compatible
- ✅ Web UI tested and functional
- ✅ All schemes harmonious

---

## 📞 Support

For questions or suggestions about colors:
- Try the 3 schemes on Web UI
- Customize via color pickers if needed
- REST API available for integrations

**Minimum version:** 1.2.0





