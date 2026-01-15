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

  server.begin();
  Serial.println("[WebServer] Started on port 80 with eye control API");
}

void handleClient() {
  if (kSystemConfig.enableWebUi) {
    server.handleClient();
  }
}

}  // namespace HttpServer
