#include "web_ui.h"
#include "web_styles.h"
#include "config.h"
#include "neopixel.h"
#include <WiFi.h>
#include <esp_system.h>

namespace WebUI {

String buildRootPage() {
  const bool connected = WiFi.isConnected();
  const String ip = connected ? WiFi.localIP().toString() : "Not connected";
  const String ssid = connected ? WiFi.SSID() : "Not connected";
  
  String html;
  html.reserve(12000);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>" + String(projectName()) + "</title>";
  html += "<style>";
  html += WebStyles::getDashboardStyles();
  html += R"rawliteral(
body { font-family: 'Segoe UI', Tahoma, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); margin: 0; padding: 20px; }
.shell { max-width: 1200px; margin: 0 auto; }
h1 { color: white; text-align: center; margin-bottom: 30px; }
.badge { background: rgba(255,255,255,0.3); padding: 5px 10px; border-radius: 5px; font-size: 0.8em; }
.section { background: white; border-radius: 15px; padding: 25px; margin-bottom: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.2); }
.section-title { font-size: 1.5em; margin-bottom: 20px; color: #333; border-bottom: 2px solid #667eea; padding-bottom: 10px; }
.grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; }
.card { background: #f8f9fa; border-radius: 10px; padding: 15px; border-left: 4px solid #667eea; }
.card strong { display: block; color: #667eea; margin-bottom: 5px; font-weight: 600; }
.card div { color: #222; font-weight: 500; word-break: break-word; }
.eye-controls { display: grid; grid-template-columns: repeat(auto-fit, minmax(120px, 1fr)); gap: 10px; margin-top: 15px; }
.eye-btn { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 15px; border-radius: 10px; cursor: pointer; font-size: 1em; transition: transform 0.2s, box-shadow 0.2s; }
.eye-btn:hover { transform: translateY(-2px); box-shadow: 0 5px 15px rgba(102,126,234,0.4); }
.eye-btn:active { transform: translateY(0); }
.eye-btn.active { background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%); box-shadow: 0 0 20px rgba(245,87,108,0.6); }
.color-picker-group { display: flex; gap: 15px; align-items: center; margin-top: 15px; flex-wrap: wrap; }
.color-group { display: flex; flex-direction: column; gap: 5px; }
.color-group label { font-weight: 600; color: #333; }
input[type="color"] { width: 60px; height: 40px; border: none; border-radius: 5px; cursor: pointer; }
input[type="range"] { width: 100%; margin-top: 10px; }
.slider-label { display: flex; justify-content: space-between; align-items: center; margin-top: 10px; font-weight: 500; color: #333; }
.switch { position: relative; display: inline-block; width: 60px; height: 34px; }
.switch input { opacity: 0; width: 0; height: 0; }
.slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 34px; }
.slider:before { position: absolute; content: ""; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; transition: .4s; border-radius: 50%; }
input:checked + .slider { background-color: #667eea; }
input:checked + .slider:before { transform: translateX(26px); }
button { background: #667eea; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; font-weight: 600; }
button:hover { background: #5568d3; }
.status-indicator { width: 12px; height: 12px; border-radius: 50%; display: inline-block; margin-right: 8px; }
.status-online { background: #4caf50; box-shadow: 0 0 8px #4caf50; }
.status-offline { background: #f44336; box-shadow: 0 0 8px #f44336; }
)rawliteral";
  html += "</style></head><body><div class=\"shell\">";
  html += "<h1><span class=\"status-indicator status-online\" id=\"statusDot\"></span>" + String(projectName()) + " <span class=\"badge\">v" + String(projectVersion()) + "</span></h1>";
  
  // Eye Control Section
  html += "<div class=\"section\"><div class=\"section-title\">🚗 Eye Animation Control</div>";
  html += "<div class=\"eye-controls\">";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(0)\">👁️ IDLE</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(1)\">😊 BLINK</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(2)\">😉 WINK LEFT</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(3)\">😉 WINK RIGHT</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(4)\">⬅️ LOOK LEFT</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(5)\">➡️ LOOK RIGHT</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(6)\">⬆️ LOOK UP</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(7)\">⬇️ LOOK DOWN</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(8)\">😄 HAPPY</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(9)\">😢 SAD</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(10)\">😠 ANGRY</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(11)\">😲 SURPRISED</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(12)\">😴 SLEEP</button>";
  html += "</div>";
  
  // Color scheme presets
  html += "<div class=\"color-scheme-group\">";
  html += "<div class=\"section-title\">🎨 Color Schemes</div>";
  html += "<button class=\"scheme-btn\" onclick=\"setScheme(0)\">🚗 Cars Orange</button>";
  html += "<button class=\"scheme-btn\" onclick=\"setScheme(1)\">👁️ Soft Realistic</button>";
  html += "<button class=\"scheme-btn\" onclick=\"setScheme(2)\">✨ Elegant Blue</button>";
  html += "</div>";
  
  // Color and brightness controls
  html += "<div class=\"color-picker-group\">";
  html += "<div class=\"color-group\"><label>Primary Color</label><input type=\"color\" id=\"primaryColor\" value=\"#FF4500\" onchange=\"setColor(1, this.value)\"></div>";
  html += "<div class=\"color-group\"><label>Secondary Color</label><input type=\"color\" id=\"secondaryColor\" value=\"#0099FF\" onchange=\"setColor(2, this.value)\"></div>";
  html += "<div class=\"color-group\"><label>Tertiary Color</label><input type=\"color\" id=\"tertiaryColor\" value=\"#000000\" onchange=\"setColor(3, this.value)\"></div>";
  html += "</div>";
  
  html += "<div class=\"slider-label\"><label>Brightness: <span id=\"brightVal\">32</span></label></div>";
  html += "<input type=\"range\" min=\"0\" max=\"255\" value=\"32\" id=\"brightness\" oninput=\"setBright(this.value)\">";
  
  html += "<div class=\"slider-label\"><label>Auto-Play Mode</label><label class=\"switch\"><input type=\"checkbox\" id=\"autoplay\" onchange=\"setAutoPlay(this.checked)\"><span class=\"slider\"></span></label></div>";
  html += "</div>";
  
  // System Info Section
  html += "<div class=\"section\"><div class=\"section-title\">⚙️ System Information</div><div class=\"grid\">";
  html += "<div class=\"card\"><strong>Processor</strong><div id=\"chip\">" + String(ESP.getChipModel()) + "</div></div>";
  html += "<div class=\"card\"><strong>CPU</strong><div id=\"cpu\">" + String(getCpuFrequencyMhz()) + " MHz</div></div>";
  html += "<div class=\"card\"><strong>Free Heap</strong><div id=\"heap\">" + String(ESP.getFreeHeap() / 1024) + " KB</div></div>";
  html += "<div class=\"card\"><strong>Free PSRAM</strong><div id=\"psram\">" + String(ESP.getFreePsram() / 1024) + " KB</div></div>";
  html += "<div class=\"card\"><strong>SSID</strong><div id=\"ssid\">" + ssid + "</div></div>";
  html += "<div class=\"card\"><strong>IP Address</strong><div id=\"ip\">" + ip + "</div></div>";
  html += "<div class=\"card\"><strong>Current Animation</strong><div id=\"currentAnim\">Loading...</div></div>";
  html += "<div class=\"card\"><strong>OTA Update</strong><div><a href=\"/update\"><button>Open OTA</button></a></div></div>";
  html += "</div></div>";
  
  // JavaScript for interactivity
  html += R"rawliteral(<script>
let currentAnimation = 0;
function updateButtons() {
  document.querySelectorAll('.eye-btn').forEach((btn, idx) => {
    btn.classList.remove('active');
    if (idx === currentAnimation) btn.classList.add('active');
  });
}
function setAnim(id) {
  currentAnimation = id;
  updateButtons();
  fetch('/api/eyes/animation?id=' + id).then(r => r.json()).then(d => console.log(d));
}
function setColor(type, hex) {
  const rgb = parseInt(hex.slice(1), 16);
  fetch('/api/eyes/color?type=' + type + '&value=' + rgb).then(r => r.json()).then(d => console.log(d));
}
function setScheme(scheme) {
  document.querySelectorAll('.scheme-btn').forEach((btn, idx) => {
    btn.classList.remove('active');
    if (idx === scheme) btn.classList.add('active');
  });
  fetch('/api/eyes/scheme?id=' + scheme).then(r => r.json()).then(d => {
    if (d.colors) {
      document.getElementById('primaryColor').value = '#' + ('000000' + d.colors[0].toString(16)).slice(-6).toUpperCase();
      document.getElementById('secondaryColor').value = '#' + ('000000' + d.colors[1].toString(16)).slice(-6).toUpperCase();
      document.getElementById('tertiaryColor').value = '#' + ('000000' + d.colors[2].toString(16)).slice(-6).toUpperCase();
    }
    console.log(d);
  });
}
function setBright(val) {
  document.getElementById('brightVal').textContent = val;
  fetch('/api/eyes/brightness?value=' + val).then(r => r.json()).then(d => console.log(d));
}
function setAutoPlay(enabled) {
  fetch('/api/eyes/autoplay?enabled=' + (enabled ? '1' : '0')).then(r => r.json()).then(d => console.log(d));
}
function updateStatus() {
  fetch('/api/status').then(r => r.json()).then(d => {
    document.getElementById('heap').textContent = Math.floor(d.heap_free / 1024) + ' KB';
    document.getElementById('psram').textContent = Math.floor(d.psram_free / 1024) + ' KB';
  }).catch(e => console.error(e));
  
  fetch('/api/eyes/status').then(r => r.json()).then(d => {
    currentAnimation = d.animation;
    updateButtons();
    const animNames = ['IDLE','BLINK','WINK LEFT','WINK RIGHT','LOOK LEFT','LOOK RIGHT','LOOK UP','LOOK DOWN','HAPPY','SAD','ANGRY','SURPRISED','SLEEP'];
    document.getElementById('currentAnim').textContent = animNames[d.animation] || 'UNKNOWN';
    document.getElementById('brightVal').textContent = d.brightness;
    document.getElementById('brightness').value = d.brightness;
    document.getElementById('autoplay').checked = d.autoplay;
    document.getElementById('primaryColor').value = '#' + d.primary_color.toString(16).padStart(6, '0');
    document.getElementById('secondaryColor').value = '#' + d.secondary_color.toString(16).padStart(6, '0');
  }).catch(e => console.error(e));
}
updateStatus();
setInterval(updateStatus, 2000);
</script></body></html>)rawliteral";
  
  return html;
}

String buildOtaPage() {
  String html;
  html.reserve(2600);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>OTA Update - ";
  html += projectName();
  html += "</title>";
  html += "<style>";
  html += WebStyles::getOtaStyles();
  html += "</style></head><body><div class=\"card\">";
  html += "<h2>OTA Update</h2><p>Select firmware (.bin) then click Upload.</p>";
  html += "<form id=\"otaForm\" method=\"POST\" action=\"/update\" enctype=\"multipart/form-data\">";
  html += "<input type=\"file\" name=\"firmware\" accept=\".bin\" required><br><br>";
  html += "<input type=\"submit\" value=\"Upload\">";
  html += "<div class=\"progress\"><div class=\"bar\" id=\"bar\"></div></div>";
  html += "<div id=\"status\">Idle</div>";
  html += "</form>";
  html += "<p><a href=\"/\"><button type=\"button\">Back to dashboard</button></a></p>";
  html += "</div>";
  html += "<script>";
  html += WebStyles::getOtaScript();
  html += "</script></body></html>";
  return html;
}

String buildStatusJson() {
  const bool connected = WiFi.isConnected();
  const size_t psramTotal = ESP.getPsramSize();
  const size_t psramFree = ESP.getFreePsram();
  
  String json = "{";
  json += "\"project\":\"" + String(projectName()) + "\",";
  json += "\"version\":\"" + String(projectVersion()) + "\",";
  json += "\"chip\":\"" + String(ESP.getChipModel()) + "\",";
  json += "\"cpu_mhz\":" + String(getCpuFrequencyMhz()) + ",";
  json += "\"heap_free\":" + String(ESP.getFreeHeap()) + ",";
  json += "\"heap_total\":" + String(ESP.getHeapSize()) + ",";
  json += "\"psram_free\":" + String(psramFree) + ",";
  json += "\"psram_total\":" + String(psramTotal) + ",";
  json += "\"ssid\":\"" + (connected ? String(WiFi.SSID()) : String("")) + "\",";
  json += "\"ip\":\"" + (connected ? WiFi.localIP().toString() : String("")) + "\"";
  json += "}";
  return json;
}

String buildEyeStatusJson() {
  #if defined(ENV_ESP32S3_N16R8)
  const NeoPixel::EyeState& state = NeoPixel::getState();
  
  String json = "{";
  json += "\"animation\":" + String(static_cast<int>(state.currentAnimation)) + ",";
  json += "\"brightness\":" + String(state.brightness) + ",";
  json += "\"primary_color\":" + String(state.primaryColor) + ",";
  json += "\"secondary_color\":" + String(state.secondaryColor) + ",";
  json += "\"autoplay\":" + String(state.autoPlay ? "true" : "false");
  json += "}";
  return json;
  #else
  return "{\"animation\":0,\"brightness\":0,\"primary_color\":0,\"secondary_color\":0,\"autoplay\":false}";
  #endif
}

}  // namespace WebUI
