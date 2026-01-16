#include "web_server.h"
#include "web_ui.h"
#include "config.h"
#include "neopixel.h"
#include <Arduino.h>
#include <Update.h>

namespace HttpServer {

static ::WebServer server(80);

void init() {
  if (!kSystemConfig.enableWebUi) {
    return;
  }

  server.on("/", []() {
    server.send(200, "text/html", WebUI::buildRootPage());
  });

  server.on("/update", HTTP_GET, []() {
    server.send(200, "text/html", WebUI::buildOtaPage());
  });

  server.on("/custom", HTTP_GET, []() {
    server.send(200, "text/html", WebUI::buildCustomEditorPage());
  });

  server.on(
    "/update", HTTP_POST,
    []() {
      const bool ok = !Update.hasError();
      server.send(ok ? 200 : 500, "text/plain", ok ? "OK" : "FAIL");
      if (ok) {
        delay(200);
        ESP.restart();
      }
    },
    []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("[OTA] Update start: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("[OTA] Update Success: %u bytes\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });

  // API endpoints for system status
  server.on("/status", []() {
    server.send(200, "application/json", WebUI::buildStatusJson());
  });

  server.on("/api/status", []() {
    server.send(200, "application/json", WebUI::buildStatusJson());
  });

  // API endpoints for eye control
  server.on("/api/eyes/status", []() {
    server.send(200, "application/json", WebUI::buildEyeStatusJson());
  });

  server.on("/api/eyes/animation", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("id")) {
      const int animId = server.arg("id").toInt();
      if (animId >= 0 && animId <= 16) {
        // TURN_LEFT (13) and TURN_RIGHT (14) use short-press behavior (duration-based timeout)
        // HAZARD (15) stays active like normal animations
        if (animId == 13 || animId == 14) {
          NeoPixel::toggleTurnSignal(static_cast<NeoPixel::AnimationType>(animId), false);
        } else {
          NeoPixel::setAnimation(static_cast<NeoPixel::AnimationType>(animId));
        }
        server.send(200, "application/json", "{\"status\":\"ok\",\"animation\":" + String(animId) + "}");
      } else {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid animation ID\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing id parameter\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/brightness", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("value")) {
      const int brightness = server.arg("value").toInt();
      if (brightness >= 0 && brightness <= 255) {
        NeoPixel::setBrightness(static_cast<uint8_t>(brightness));
        server.send(200, "application/json", "{\"status\":\"ok\",\"brightness\":" + String(brightness) + "}");
      } else {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid brightness value\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing value parameter\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/signal_duration", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("value")) {
      const int val = server.arg("value").toInt();
      if (val >= 100 && val <= 60000) {
        NeoPixel::setSignalDuration((uint16_t)val);
        server.send(200, "application/json", "{\"status\":\"ok\",\"signal_duration\":" + String(val) + "}");
      } else {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid duration\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing value parameter\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/color", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("type") && server.hasArg("value")) {
      const int colorType = server.arg("type").toInt();
      const uint32_t colorValue = (uint32_t)server.arg("value").toInt();
      
      if (colorType == 1) {
        NeoPixel::setPrimaryColor(colorValue);
        server.send(200, "application/json", "{\"status\":\"ok\",\"color_type\":\"primary\"}");
      } else if (colorType == 2) {
        NeoPixel::setSecondaryColor(colorValue);
        server.send(200, "application/json", "{\"status\":\"ok\",\"color_type\":\"secondary\"}");
      } else if (colorType == 3) {
        NeoPixel::setTertiaryColor(colorValue);
        server.send(200, "application/json", "{\"status\":\"ok\",\"color_type\":\"tertiary\"}");
      } else {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid color type\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing parameters\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/scheme", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("id")) {
      const int schemeId = server.arg("id").toInt();
      NeoPixel::applyColorScheme((NeoPixel::ColorScheme)schemeId);
      
      String response = "{\"status\":\"ok\",\"scheme\":" + String(schemeId);
      response += ",\"colors\":[";
      
      // Get current colors and send them back
      const NeoPixel::EyeState& state = NeoPixel::getState();
      response += String(state.primaryColor) + "," + String(state.secondaryColor) + "," + String(state.tertiaryColor);
      response += "]}";
      
      server.send(200, "application/json", response);
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing scheme id\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/autoplay", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("enabled")) {
      const bool enabled = server.arg("enabled") == "1";
      NeoPixel::setAutoPlay(enabled);
      server.send(200, "application/json", "{\"status\":\"ok\",\"autoplay\":" + String(enabled ? "true" : "false") + "}");
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing enabled parameter\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/off", []() {
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::setAutoPlay(false);  // Disable autoplay to keep LEDs off
    NeoPixel::clear();
    NeoPixel::show();
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"LEDs OFF, AutoPlay disabled\"}");
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/custom/pixel", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("eye") && server.hasArg("x") && server.hasArg("y") && server.hasArg("color")) {
      const String eye = server.arg("eye");
      const int x = server.arg("x").toInt();
      const int y = server.arg("y").toInt();
      const uint32_t color = (uint32_t)server.arg("color").toInt();
      
      if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        if (eye == "left") {
          NeoPixel::setPixelLeft(x, y, color);
        } else if (eye == "right") {
          NeoPixel::setPixelRight(x, y, color);
        } else {
          server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid eye\"}");
          return;
        }
        server.send(200, "application/json", "{\"status\":\"ok\",\"x\":" + String(x) + ",\"y\":" + String(y) + "}");
      } else {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Coordinates out of range\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing parameters\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/eyes/custom/clear", []() {
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::clear();
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Custom pattern cleared\"}");
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/custom/apply", []() {
    #if defined(ENV_ESP32S3_N16R8)
    if (server.hasArg("eye") && server.hasArg("pattern_left") && server.hasArg("pattern_right")) {
      String eyeStr = server.arg("eye");
      String patternLeftStr = server.arg("pattern_left");
      String patternRightStr = server.arg("pattern_right");
      
      // Parse left pattern: comma-separated values
      uint32_t patternLeft[64] = {0};
      int indexL = 0, startL = 0;
      for (int i = 0; i <= patternLeftStr.length() && indexL < 64; i++) {
        if (patternLeftStr[i] == ',' || i == patternLeftStr.length()) {
          int end = (i == patternLeftStr.length()) ? i : i;
          String colorStr = patternLeftStr.substring(startL, end);
          patternLeft[indexL] = (uint32_t)strtol(colorStr.c_str(), NULL, 0);
          indexL++;
          startL = i + 1;
        }
      }
      
      // Parse right pattern: comma-separated values
      uint32_t patternRight[64] = {0};
      int indexR = 0, startR = 0;
      for (int i = 0; i <= patternRightStr.length() && indexR < 64; i++) {
        if (patternRightStr[i] == ',' || i == patternRightStr.length()) {
          int end = (i == patternRightStr.length()) ? i : i;
          String colorStr = patternRightStr.substring(startR, end);
          patternRight[indexR] = (uint32_t)strtol(colorStr.c_str(), NULL, 0);
          indexR++;
          startR = i + 1;
        }
      }
      
      // Save patterns to NeoPixel persistent storage
      NeoPixel::setCustomPatternLeft(patternLeft);
      NeoPixel::setCustomPatternRight(patternRight);
      
      // Apply to selected eye(s)
      if (eyeStr == "left" || eyeStr == "both") {
        for (int i = 0; i < 64; i++) {
          int y = i / 8;
          int x = i % 8;
          NeoPixel::setPixelLeft(x, y, patternLeft[i]);
        }
      }
      if (eyeStr == "right" || eyeStr == "both") {
        for (int i = 0; i < 64; i++) {
          int y = i / 8;
          int x = i % 8;
          NeoPixel::setPixelRight(x, y, patternRight[i]);
        }
      }
      
      NeoPixel::show();
      NeoPixel::setAnimation(NeoPixel::AnimationType::CUSTOM);
      
      server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Custom pattern applied\"}");
    } else if (server.hasArg("eye") && server.hasArg("pattern")) {
      // Legacy single pattern support
      String eyeStr = server.arg("eye");
      String patternStr = server.arg("pattern");
      
      uint32_t pattern[64] = {0};
      int index = 0, start = 0;
      for (int i = 0; i <= patternStr.length() && index < 64; i++) {
        if (patternStr[i] == ',' || i == patternStr.length()) {
          int end = (i == patternStr.length()) ? i : i;
          String colorStr = patternStr.substring(start, end);
          pattern[index] = (uint32_t)strtol(colorStr.c_str(), NULL, 0);
          index++;
          start = i + 1;
        }
      }
      
      if (eyeStr == "left" || eyeStr == "both") {
        for (int i = 0; i < 64; i++) {
          int y = i / 8;
          int x = i % 8;
          NeoPixel::setPixelLeft(x, y, pattern[i]);
        }
      }
      if (eyeStr == "right" || eyeStr == "both") {
        for (int i = 0; i < 64; i++) {
          int y = i / 8;
          int x = i % 8;
          NeoPixel::setPixelRight(x, y, pattern[i]);
        }
      }
      
      NeoPixel::show();
      NeoPixel::setAnimation(NeoPixel::AnimationType::CUSTOM);
      
      server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Custom pattern applied\"}");
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing pattern parameters\"}");
    }
    #else
    server.send(501, "application/json", "{\"status\":\"error\",\"message\":\"NeoPixel not available\"}");
    #endif
  });

  server.on("/api/custom/get", []() {
    // Return saved patterns as JSON (decimal format)
    const uint32_t* customLeft = NeoPixel::getCustomPatternLeft();
    const uint32_t* customRight = NeoPixel::getCustomPatternRight();
    
    String response = "{\"status\":\"ok\",\"left\":[";
    for (int i = 0; i < 64; i++) {
      response += String(customLeft[i]);
      if (i < 63) response += ",";
    }
    response += "],\"right\":[";
    for (int i = 0; i < 64; i++) {
      response += String(customRight[i]);
      if (i < 63) response += ",";
    }
    response += "]}";
    server.send(200, "application/json", response);
  });

  server.begin();
  Serial.println("[WebServer] Started on port 80 with eye control API");
}

void handleClient() {
  if (kSystemConfig.enableWebUi) {
    server.handleClient();
  }
}

}  // namespace HttpServer
