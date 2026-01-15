# NeoPixel Patterns Documentation - Animated Eyes

**Version:** 1.5.9  
**Minimum version:** 1.0.0  
**Language:** English  
**Revision Date:** January 14, 2026  
**Improvements:** Realistic patterns with 3D shadow effects

---

## System Overview

### Color Architecture
The NeoPixel display system uses **3 color levels** to create realistic and depth effects:

| Level | Code | Description |
|-------|------|-------------|
| **0** | `0` | Black / Off - Pixel disabled |
| **1** | `1` | Primary color - Main iris/sclera |
| **2** | `2` | Secondary color - Highlights/eyelids/reflections |
| **3** | `3` | Tertiary color - Shadows/pupil/depth |

### Default Color Configuration
```cpp
primaryColor   = 0x00FF4500   // Bright orange (iris - Cars style)
secondaryColor = 0x00FFFFFF   // White (highlights, eyelids)
tertiaryColor  = 0x001a0900   // Dark brown/black (shadows, pupil)
```

---

## Pattern Catalog (Numbered)

### **#01 - PATTERN_IDLE: Resting Eyes**
**State:** Centered gaze, neutral  
**Usage:** Default display on startup

```
0,0,0,0,0,0,0,0
0,0,2,2,2,2,0,0    ← Upper eyelid (highlight)
0,2,1,3,3,1,2,0    ← Upper iris with dark center
0,2,1,3,3,1,2,0    ← Iris center (pupil = 3)
0,2,1,1,1,1,2,0    ← Lower iris with lash traces
0,0,2,2,2,2,0,0    ← Lower eyelid
0,0,0,1,1,0,0,0    ← Lower lash line detail
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Depth: Dark pupil (3) surrounded by iris (1)
- ✓ Lighting: Highlights (2) on eyelids
- ✓ Detail: Lash traces at bottom
- ✓ Realism: 3D effect with gradation

---

### **#02 - PATTERN_SLEEP: Closed Eyes**
**State:** Eyelids fully closed  
**Usage:** Sleep mode, inactive startup

```
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,2,2,2,2,2,2,0    ← Upper eyelid (highlight)
0,1,1,1,1,1,1,0    ← Main lid line (coloration)
0,1,1,1,1,1,1,0    ← 3D thickness
0,2,2,2,2,2,2,0    ← Lower eyelid (highlight)
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ 3D Effect: Thick lid line (3D impression)
- ✓ Highlights: Top/bottom for relief
- ✓ Smooth transitions

---

### **#03 - PATTERN_BLINK: Blinking Animation**
**State:** Progressive closure  
**Duration:** Quick (~100ms)

```
0,0,0,0,0,0,0,0
0,0,2,2,2,2,0,0    ← Descending eyelid
0,2,2,1,1,2,2,0    ← Mid-blink compression
0,2,2,1,1,2,2,0    ← Compressed iris
0,2,2,2,2,2,2,0    ← Eyelid compression
0,0,2,2,2,2,0,0    ← Lower eyelid
0,0,0,1,1,0,0,0    ← Slight gap
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Dynamic: Compressed iris (realistic effect)
- ✓ Shading: Smooth transition
- ✓ Animation: Creates organic movement

---

### **#04 - PATTERN_LOOK_LEFT: Looking Left**
**State:** Pupil shifted left  
**Angle:** ~90° left

```
0,0,0,0,0,0,0,0
0,0,2,2,1,0,0,0    ← Eyelid with iris at edge
2,2,3,3,1,2,0,0    ← Left-shifted pupil (3 = shadow)
2,2,3,3,1,2,0,0    ← Pupil center (intense)
2,2,1,1,1,2,0,0    ← Lower iris
0,0,2,2,1,0,0,0    ← Lower eyelid
0,0,0,1,0,0,0,0    ← Lash detail
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Left shift: Pupil at edge
- ✓ Side shading: Depth with (3)
- ✓ Realism: Outer eyelid disappears

---

### **#05 - PATTERN_LOOK_RIGHT: Looking Right**
**State:** Pupil shifted right  
**Angle:** ~90° right

```
0,0,0,0,0,0,0,0
0,0,0,1,2,2,2,0    ← Eyelid with iris at edge
0,0,1,1,3,3,2,2    ← Right-shifted pupil (3 = shadow)
0,0,1,1,3,3,2,2    ← Pupil center
0,0,1,1,1,2,2,2    ← Lower iris
0,0,0,1,2,2,2,0    ← Lower eyelid
0,0,0,0,1,0,0,0    ← Lash detail
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Mirror of LOOK_LEFT
- ✓ Pupil at right edge
- ✓ Opposite shading

---

### **#06 - PATTERN_LOOK_UP: Looking Up**
**State:** Pupil shifted upward  
**Angle:** ~90° up

```
0,0,2,2,2,2,0,0    ← Raised upper eyelid
0,2,3,3,3,3,2,0    ← Upper pupil (dilated)
0,2,3,3,3,3,2,0    ← Pupil center (intense)
0,2,1,1,1,1,2,0    ← Lower iris portion
0,0,2,2,2,2,0,0    ← Lower eyelid
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Upper eyelid lifted high
- ✓ Pupil toward top
- ✓ Empty spaces below

---

### **#07 - PATTERN_LOOK_DOWN: Looking Down**
**State:** Pupil shifted downward  
**Angle:** ~90° down

```
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,2,2,2,2,0,0    ← Upper eyelid
0,2,1,1,1,1,2,0    ← Upper iris portion
0,2,3,3,3,3,2,0    ← Pupil center (toward down)
0,2,3,3,3,3,2,0    ← Lower pupil (intense)
0,0,2,2,2,2,0,0    ← Lower eyelid lowered
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Mirror of LOOK_UP
- ✓ Lower eyelid lowered
- ✓ Pupil toward bottom

---

### **#08 - PATTERN_HAPPY: Joyful Expression**
**State:** Wide-open eyes, happy  
**Pupils:** Widely dilated

```
0,0,2,2,2,2,0,0    ← Upper eyelid curved
0,2,1,1,1,1,2,0    ← Wide sclera
2,1,3,3,2,3,1,2    ← Large wide-open iris
2,1,3,3,2,3,1,2    ← Center: light reflection (2) = joy
2,1,3,3,2,3,1,2    ← Iris continues - very open eyes
0,2,1,1,1,1,2,0    ← Lower sclera
0,0,2,2,2,2,0,0    ← Lower eyelid curved
0,0,0,2,2,0,0,0    ← Joy wrinkles (crow's feet)
```

**Features:**
- ✓ Large pupils: Maximum dilation
- ✓ Central reflection: Joy light (2)
- ✓ Wrinkles: Mimicry detail
- ✓ Realism: Sparkling eyes

---

### **#09 - PATTERN_SAD: Sad Expression**
**State:** Sad eyes, droopy eyelids  
**Pupils:** Compressed downward

```
0,2,2,2,2,2,0,0    ← Upper eyelid drooping left
2,1,1,1,1,1,2,0    ← Drooping sclera
1,3,3,3,3,1,1,0    ← Compressed iris (melancholy)
1,3,3,3,3,1,1,0    ← Small pupil (closed)
0,2,1,1,1,2,0,0    ← Raised lower eyelid (sad angle)
0,0,2,2,2,0,0,0    ← Eye corner droops
0,0,0,1,0,0,0,0    ← Tear detail
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Compression: Compressed iris
- ✓ Angle: Drooping eyelids
- ✓ Detail: Possible tear
- ✓ Tone: Dark and introspective

---

### **#10 - PATTERN_ANGRY: Furious Expression**
**State:** Squinted eyes, aggressive  
**Pupils:** Compressed, intense stare

```
1,2,2,1,1,0,0,0    ← Upper eyelid angled
2,1,3,3,1,2,0,0    ← Compressed iris (fury)
2,1,3,3,1,2,0,0    ← Dark intense pupil (3)
1,1,1,1,1,1,0,0    ← Maximum compactness
0,2,2,1,1,0,0,0    ← Lower eyelid angled
0,0,2,1,0,0,0,0    ← Aggressive angle
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Squinting: Asymmetric angles
- ✓ Compactness: Very compressed iris
- ✓ Shading: Maximum (3) in center
- ✓ Aggression: Piercing stare

---

### **#11 - PATTERN_SURPRISED: Surprised Expression**
**State:** Extremely wide-open eyes  
**Pupils:** Maximally dilated

```
0,2,2,2,2,2,0,0    ← Very raised upper eyelid
2,1,3,3,3,1,2,0    ← Giant dilated iris
1,3,3,2,2,3,3,1    ← Huge pupil (2 = shock reflection)
1,3,3,2,2,3,3,1    ← Widely open pupil
2,1,3,3,3,1,2,0    ← Iris continues - alert state
0,2,2,2,2,2,0,0    ← Very lowered lower eyelid
0,0,2,2,2,0,0,0    ← Extreme expressivity
0,0,0,0,0,0,0,0
```

**Features:**
- ✓ Extreme dilation: Maximum possible
- ✓ Reflections: (2) = shock light
- ✓ Opening: Eyelids at extremes
- ✓ Emotional: Shock/astonishment

---

## Recommended Color Schemes

### Configuration #1: Cars (Bright Orange)
```cpp
primaryColor   = 0x00FF4500   // Pixar orange
secondaryColor = 0x00FFFFFF   // Pure white
tertiaryColor  = 0x001a0900   // Dark brown
```

### Configuration #2: Night Mode (Blue)
```cpp
primaryColor   = 0x000066FF   // Bright blue
secondaryColor = 0x0099DDFF   // Light blue
tertiaryColor  = 0x00001155   // Very dark blue
```

### Configuration #3: Friendly Mode (Green)
```cpp
primaryColor   = 0x0033FF33   // Lime green
secondaryColor = 0x00DDFFDD   // Very light green
tertiaryColor  = 0x00113311   // Very dark green
```

---

## Usage in Code

### Change animation
```cpp
// In web_ui.cpp or other controller
NeoPixel::setAnimation(AnimationType::HAPPY);  // #08 Happy
```

### Change colors
```cpp
// Primary color (iris)
NeoPixel::setPrimaryColor(NeoPixel::makeColor(255, 69, 0));  // Orange

// Secondary color (highlights)
NeoPixel::setSecondaryColor(NeoPixel::makeColor(255, 255, 255));  // White

// Tertiary color (shadows)
NeoPixel::setTertiaryColor(NeoPixel::makeColor(26, 9, 0));  // Brown
```

### Enable autoplay
```cpp
NeoPixel::setAutoPlay(true);  // Alternates randomly between animations
```

---

## Technical Notes

### Resolution
- **Matrix:** 8×8 pixels (64 LEDs per eye)
- **Colors per pixel:** 3 bits (0-7 levels theoretical, using 0-3)
- **Depth:** 24-bit RGB (8 bits per channel)

### Performance
- **Display time:** <10ms per eye
- **Refresh rate:** ~100 Hz
- **Pattern memory:** ~11 KB (11 patterns × 64 bytes)

### Future Animation Ideas
- Automatic blinking
- Smooth transitions between states
- Real-time reactions
- Dynamic AI-driven expressions

---

## Improvements Made (01/14/2026)

| Before | After |
|--------|-------|
| Simple 2-color patterns | Realistic 3-color patterns |
| No 3D depth | Realistic shading & highlights |
| Missing documentation | Complete numbered documentation |
| No shadow effects | Dynamic shadows & highlights |
| Static expressions | Nuanced emotional expressions |

---

## Source Files
[neopixel.cpp](../src/neopixel.cpp) - Pattern definitions  
[neopixel.h](../include/neopixel.h) - Public interface
