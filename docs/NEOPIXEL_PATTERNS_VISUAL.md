# NeoPixel Patterns Visualization - Graphical Guide

**Version:** 1.6.0  
**Minimum version:** 1.6.0  
**Language:** English  
**Date:** January 16, 2026  

Visual reference for the 17 active animations using the 8-color palette engine.

## Symbol Legend (Palette Codes)
```
0   = .   (off)
C1  = palette slot 1 (iris core / enforced amber for signals)
C2  = slot 2
C3  = slot 3
C4  = slot 4 (highlight)
C5  = slot 5 (cool highlight)
C6  = slot 6 (deep shadow/contrast)
C7  = slot 7 (outline/amber depth)
C8  = slot 8 (black/pupil)
Suffixes: h=70%, m=40%, l=20% (e.g., C7h = slot 7 at 70%)
100 (arrows) = forced C1 (amber override)
```

---

## #01 - IDLE (Eyes at Rest)
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C5 C1h C1 C1 C1h C5 C5h
C5 C1h C8 C8h C8h C8 C1h C5
C5 C1h C8h C8m C8m C8h C1h C5
C5h C5 C1h C1 C1 C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #02 - SLEEP (Closed)
```
0 C7h C7h C7h C7h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C7h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C7h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #03 - BLINK (Closing)
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C5 C1h C1 C1 C1h C5 C5h
C5 C1h C8 C8h C8h C8 C1h C5
C5 C1h C8h C8m C8m C8h C1h C5
C5h C5 C1h C1 C1 C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```
(Frame 1/2 compress further to a slit.)

## #04 - LOOK_LEFT (Gaze Left)
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C5h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #05 - LOOK_RIGHT (Gaze Right)
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C7h C5h C4h C1h C5 C5h
0 C7h C7h C5 C8h C8 C1 C5
0 C7h C7h C5 C8 C8h C8 C5
0 C7h C7h C5h C4h C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #06 - LOOK_UP (Gaze Up)
```
0 C7h C7h C7h C7h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C7h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C7h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #07 - LOOK_DOWN (Gaze Down)
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C4h C1h C5 C5h C7h C7h 0
C5 C1 C8 C8h C5 C7h C7h 0
C5 C8 C8h C8 C5 C7h C7h 0
C5h C4h C1h C5 C5h C7h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C7h C7h C7h C7h C7h 0
```

## #08 - HAPPY
```
0 C7h C5h C5 C5 C5h C7h 0
0 C5h C5 C4h C4h C5 C5h 0
C5h C4h C1h C1 C1 C1h C4h C5h
C4h C1h C8 C8h C8h C8 C1h C4h
C4h C1 C8 C8h C8h C8 C1 C4h
C5h C4h C1h C1 C1 C1h C4h C5h
C7h C5 C4h C1h C1h C4h C5 C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #09 - SAD
```
0 C7h 0 0 0 0 C7h 0
C7h C7h C7 0 0 C7 C7h C7h
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C4h C1 C1 C4h C5h C7h
C5 C4h C1 C8 C8 C1 C4h C5
C4h C1 C8 C8h C8h C8 C1 C4h
C7h C5h C5h C4h C4h C5h C5h C7h
0 0 C7h C5h C5h C7h 0 0
```

## #10 - ANGRY
```
0 0 0 0 0 0 0 0
0 0 C7h C5h C5h C7h 0 0
0 0 C5 C4h C4h C5 C7h 0
C5h C5 C8 C8h C8h C8 C5 C5h
C5 C4h C8 C8m C8m C8 C4h C5
C5h C4h C1 C8 C8 C1 C4h C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #11 - SURPRISED
```
0 C7h C5h C5 C5 C5h C7h 0
C7h C5h C5 C5 C5 C5 C5h C7h
C5h C5 C1h C1 C1 C1h C5 C5h
C5 C4h C1 C1h C1h C1 C4h C5
C5 C4h C1 C1h C1h C1 C4h C5
C5h C5 C1h C1 C1 C1h C5 C5h
C7h C5h C5 C5 C5 C5 C5h C7h
0 C7h C5h C5 C5 C5h C7h 0
```

## #12 - WINK_LEFT
Left eye: BLINK sequence; Right eye: IDLE.

## #13 - WINK_RIGHT
Left eye: IDLE; Right eye: BLINK sequence.

## #14 - TURN_LEFT (Arrow Amber)
```
0 0 A 0 0 0 0 0
0 A A 0 0 0 0 0
0 A A A A A A A
A A A A A A A A
A A A A A A A A
0 A A A A A A A
0 A A 0 0 0 0 0
0 0 A 0 0 0 0 0
```
(A = forced amber via code 100 → C1)

## #15 - TURN_RIGHT (Arrow Amber)
```
0 0 0 0 0 A 0 0
0 0 0 0 0 A A 0
A A A A A A A 0
A A A A A A A A
A A A A A A A A
A A A A A A A 0
0 0 0 0 0 A A 0
0 0 0 0 0 A 0 0
```

## #16 - HAZARD
Left eye: TURN_LEFT arrow; Right eye: TURN_RIGHT arrow (both amber, steady until another animation is selected).

## #17 - CUSTOM
User-defined palette-coded 8×8 per eye. Codes follow the legend above; dim by adding `h/m/l` (70/40/20%).

---

## Pattern #15 - TURN_LEFT (Arrow Left - Turn Signal)

```
. . ▓ . . . . .
. ▓ ▓ . . . . .
▓ ▓ ▓ ▓ ▓ ▓ ▓ .
▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓
▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓
. ▓ ▓ ▓ ▓ ▓ ▓ ▓
. ▓ ▓ . . . . .
. . ▓ . . . . .
```

**Note:** Left-pointing arrow using amber/primary color for turn signals
- Special pixel code (100) renders as primary color
- Fills entire matrix width for automotive visibility

---

## Pattern #16 - TURN_RIGHT (Arrow Right - Turn Signal)

```
. . . . . . ▓ . .
. . . . . . ▓ ▓ .
. ▓ ▓ ▓ ▓ ▓ ▓ ▓
▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓
▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓
▓ ▓ ▓ ▓ ▓ ▓ ▓ .
. . . . . . ▓ ▓ .
. . . . . . ▓ . .
```

**Note:** Right-pointing arrow using amber/primary color for turn signals
- Special pixel code (100) renders as primary color  
- Fills entire matrix width for automotive visibility

---

## Pattern #17 - HAZARD (Both Arrows - Hazard Warning)

When activated, both eyes display simultaneously:
```
Left Eye (TURN_LEFT):     Right Eye (TURN_RIGHT):
. . ▓ . . . . .          . . . . . . ▓ . .
. ▓ ▓ . . . . .          . . . . . . ▓ ▓ .
▓ ▓ ▓ ▓ ▓ ▓ ▓ .         . ▓ ▓ ▓ ▓ ▓ ▓ ▓
▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓          ▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓
▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓          ▓ ▓ ▓ ▓ ▓ ▓ ▓ ▓
. ▓ ▓ ▓ ▓ ▓ ▓ ▓          ▓ ▓ ▓ ▓ ▓ ▓ ▓ .
. ▓ ▓ . . . . .          . . . . . . ▓ ▓ .
. . ▓ . . . . .          . . . . . . ▓ . .
```

**Note:** Both arrows displayed simultaneously for hazard/warning mode
- Activated by simultaneous button press (Button 1 + Button 2)
- Creates bi-directional warning effect

---

## Visual Comparison: BEFORE → AFTER

### BEFORE (2 colors, flat)
```
Pattern IDLE (old):
. . . . . . . .
. . 1 1 1 1 . .
. 1 2 2 2 2 1 .
. 1 2 2 2 2 1 .
. 1 2 2 2 2 1 .
. 1 1 1 1 1 1 .
. . 1 1 1 1 . .
. . . . . . . .

= 2 levels: Eyelid(1) + Center(2)
```

### AFTER (3 colors, 3D)
```
Pattern IDLE (new):
. . . . . . . .
. . ◇ ◇ ◇ ◇ . .
. ◇ ◉ ● ● ◉ ◇ .
. ◇ ◉ ● ● ◉ ◇ .
. ◇ ◉ ◉ ◉ ◉ ◇ .
. . ◇ ◇ ◇ ◇ . .
. . . ◉ ◉ . . .
. . . . . . . .

= 3 levels + details:
  - Eyelid(◇) with highlights
  - Natural iris(◉)
  - Dark pupil(●)
  - Lashes at bottom
```

**Gain:** +40% visual detail

---

## Color Schemes

### Configuration #1: Orange (Cars)
- ◉ Primary = RGB(255, 69, 0) - Pixar Orange
- ◇ Secondary = RGB(255, 255, 255) - Pure White
- ● Tertiary = RGB(26, 9, 0) - Very Dark Brown

### Configuration #2: Blue (Night)
- ◉ Primary = RGB(0, 102, 255) - Bright Blue
- ◇ Secondary = RGB(153, 221, 255) - Light Blue
- ● Tertiary = RGB(0, 17, 85) - Very Dark Blue

### Configuration #3: Green (Friendly)
- ◉ Primary = RGB(51, 255, 51) - Lime Green
- ◇ Secondary = RGB(221, 255, 221) - Very Light Green
- ● Tertiary = RGB(17, 51, 17) - Very Dark Green

---

## Usage Recommendations

| Pattern | Context | Typical Duration |
|---------|---------|------------------|
| IDLE | Default display | Continuous |
| SLEEP | Sleep mode | Continuous |
| BLINK | Fast animation | 100-200ms |
| LOOK_* | Navigation/attention | 1-2 seconds |
| HAPPY | Positive confirmation | 1-2 seconds |
| SAD | Error/regret | 2-3 seconds |
| ANGRY | Alert/urgency | 2-3 seconds |
| SURPRISED | Surprise notification | 1-2 seconds |

---

**Generated:** January 14, 2026  
**Format:** ASCII Art visualization  
**Resolution:** 8×8 pixels per eye





