# Cars Eyes

> **Version:** 1.5.20  
> **Minimum Version:** 1.0.0

**Animated eye system using two 8x8 NeoPixel matrices for ESP32-S3, styled like Cars movie characters.**

![ESP32-S3](https://img.shields.io/badge/ESP32-S3-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-ready-green)
![License](https://img.shields.io/badge/license-MIT-blue)

## 🎯 Features

- **Dual 8x8 NeoPixel Matrix Eyes** - Left and right eye control with independent or synchronized animations
- **13 Built-in Animations** - IDLE, BLINK, WINK, LOOK (left/right/up/down), HAPPY, SAD, ANGRY, SURPRISED, SLEEP
- **Turn Signals & Hazards** - Button-controlled Left/Right automotive signals and Hazard warning mode.
- **Settings Persistence** - Automatically saves brightness, color, scheme, and signal duration to memory.
- **Modern Web UI** - Real-time control interface without page reloads
- **Auto-Play Mode** - Random animation sequences for lifelike behavior
- **Color Customization** - Primary and secondary color control via web interface
- **Brightness Control** - Adjustable brightness (0-255) for both eyes
- **Status LED** - Built-in RGB LED for WiFi status indication
- **LCD Display** - Real-time system information on ST7789 screen
- **OTA Updates** - Wireless firmware updates via web interface
- **REST API** - Complete API for programmatic control

## 📋 Hardware Requirements

### Required Components

- **ESP32-S3 DevKitC-1 N16R8** (with 16MB Flash + 8MB PSRAM)
- **2x NeoPixel 8x8 Matrix** (WS2812B, 64 LEDs each)
- **ST7789 LCD Display** (240x320 or 240x240)
- **5V Power Supply** (minimum 2A, recommended 5A for maximum brightness)
- **2x 330Ω Resistors** (for NeoPixel data lines)
- **1000µF Electrolytic Capacitor** (16V+, for power stabilization)
- **2x 100nF Ceramic Capacitors** (for each matrix, HF filtering)
- **3x Push Buttons** (optional, for manual control)

### GPIO Pinout (ESP32-S3)

| Component | GPIO | Description |
|-----------|------|-------------|
| Left Eye Matrix | GPIO4 | NeoPixel data line (DIN) |
| Right Eye Matrix | GPIO5 | NeoPixel data line (DIN) |
| Status LED | GPIO48 | Built-in RGB LED |
| LCD SCK | GPIO12 | SPI clock |
| LCD MOSI | GPIO11 | SPI data |
| LCD CS | GPIO10 | Chip select |
| LCD DC | GPIO9 | Data/Command |
| LCD RST | GPIO8 | Reset |
| LCD BL | GPIO7 | Backlight (Digital) |
| Button BOOT | GPIO0 | Built-in boot button |
| Button User 1 | GPIO16 | Custom button 1 |
| Button User 2 | GPIO17 | Custom button 2 |

**⚠️ IMPORTANT:** Do NOT power the NeoPixel matrices from the ESP32's 5V pin. Use an external 5V power supply with sufficient current capacity (minimum 2A, up to 8A for full white at maximum brightness).

## 🔌 Wiring Diagram

```
5V Power Supply
    │
    ├──[1000µF]──GND
    │
    ├────────────► VCC Left Matrix
    │             │
    │             └──[100nF]──GND
    │
    └────────────► VCC Right Matrix
                  │
                  └──[100nF]──GND

ESP32-S3
    GPIO4 ──[330Ω]──► DIN Left Matrix
    GPIO5 ──[330Ω]──► DIN Right Matrix
    GPIO48 ─────────► Built-in RGB LED
    GND ────────────► GND (all components, common ground)
```

## 🚀 Quick Start

### 1. Setup Environment

```bash
# Clone repository
git clone <your-repository-url>
cd Cars_Eyes

# Install PlatformIO (if not already installed)
pip install platformio

# Install dependencies
pio pkg install
```

### 2. Configure WiFi

Copy `include/secrets_example.h` to `include/secrets.h` and edit:

```cpp
#define WIFI_SSID "YourNetworkName"
#define WIFI_PASSWORD "YourPassword"
```

### 3. Build & Upload

```bash
# For ESP32-S3 (default)
pio run -e esp32s3_n16r8 -t upload

# Monitor serial output
pio device monitor
```

### 4. Access Web Interface

After successful boot, the LCD will display the IP address. Access the web interface:

```
http://<ESP32-IP-ADDRESS>/
```

Or use mDNS (if supported by your network):

```
http://neopixel-eyes.local/
```

## 🌐 Web Interface

The web UI provides complete control over the eye animations:

### Animation Controls
- 13 animation buttons with real-time preview
- Primary and secondary color pickers
- Brightness slider (0-255)
- Auto-play toggle for random animations

### System Information
- WiFi status and network info
- Memory usage (Heap and PSRAM)
- Current animation and brightness
- OTA update interface

### API Endpoints

**Get System Status:**
```http
GET /api/status
```

**Get Eye Status:**
```http
GET /api/eyes/status
```

**Set Animation:**
```http
GET /api/eyes/animation?id=0
```
- `id`: 0=IDLE, 1=BLINK, 2=WINK_LEFT, 3=WINK_RIGHT, 4=LOOK_LEFT, 5=LOOK_RIGHT, 6=LOOK_UP, 7=LOOK_DOWN, 8=HAPPY, 9=SAD, 10=ANGRY, 11=SURPRISED, 12=SLEEP

**Set Brightness:**
```http
GET /api/eyes/brightness?value=128
```
- `value`: 0-255

**Set Color:**
```http
GET /api/eyes/color?type=1&value=16744448
```
- `type`: 1=primary, 2=secondary
- `value`: RGB color as integer (0x00RRGGBB)

**Set Auto-Play:**
```http
GET /api/eyes/autoplay?enabled=1
```
- `enabled`: 0=off, 1=on

## 📖 Documentation

Complete documentation is available in the `docs/` folder:

- [Getting Started Guide](docs/GETTING_STARTED.md)
- [Project Structure](docs/PROJECT_STRUCTURE.md)
- [Pin Mapping Reference](docs/PIN_MAPPING.md)
- [Adding New Modules](docs/ADD_MODULES.md)
- [Web UI Documentation](docs/WEB_UI.md)
- [User Guide](docs/USER_GUIDE.md)

French versions are also available (suffix `_FR.md`).

## ⚙️ Configuration

Edit `include/config.h` to customize:

```cpp
constexpr SystemConfig kSystemConfig {
  .enableWebUi = true,        // Enable web interface
  .enableOta = true,          // Enable OTA updates
  .backlightLevel = 200,      // LCD brightness (0-255)
  .lcdRotation = 0,           // LCD rotation (0-3)
  .enableBootBar = true,      // Show boot progress bar
  .mdnsHost = "neopixel-eyes" // mDNS hostname
};
```

## 🎨 Customizing Animations

Animation patterns are defined in `src/neopixel.cpp`. Each pattern is an 8x8 array:

```cpp
// Pattern values:
// 0 = OFF (black)
// 1 = Primary color (outline)
// 2 = Secondary color (fill)

static const uint8_t PATTERN_CUSTOM[64] = {
  0,0,0,0,0,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,2,2,2,2,1,0,
  0,1,2,2,2,2,1,0,
  0,1,2,2,2,2,1,0,
  0,1,1,1,1,1,1,0,
  0,0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,0
};
```

Add your pattern to the `getPatternForAnimation()` function to use it.

## 🔧 Troubleshooting

**Eyes not lighting up:**
- Check power supply (5V, sufficient current)
- Verify GPIO connections (GPIO4 and GPIO5)
- Confirm common ground between ESP32 and power supply
- Check resistors on data lines (330Ω)

**Web UI not accessible:**
- Verify WiFi credentials in `secrets.h`
- Check serial monitor for IP address
- Try mDNS: `http://neopixel-eyes.local`
- Ensure router allows local device communication

**LCD not displaying:**
- Verify SPI connections (GPIO 8-12)
- Check LCD power (3.3V recommended)
- Adjust `lcdRotation` in config.h if display is upside down

**Flickering animations:**
- Add capacitors (1000µF on power, 100nF per matrix)
- Reduce brightness in high-current situations
- Use shorter wires for data lines
- Ensure stable 5V power supply

## 📜 License

This project is licensed under the MIT License - see the [LICENCE](LICENCE) file for details.

## 🙏 Credits

- **Adafruit NeoPixel Library** - LED control
- **Adafruit GFX & ST7789 Libraries** - LCD display
- **ESP32 Arduino Core** - ESP32 framework

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📞 Support

For questions and support, please open an issue on GitHub.

---

**Built with ❤️ for the maker community**


