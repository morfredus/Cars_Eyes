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
.blinking { animation: blinker 1.5s linear infinite; }
@keyframes blinker { 0% { opacity: 1; } 50% { opacity: 0; } 100% { opacity: 1; } }
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
  html += "<button class=\"eye-btn\" onclick=\"setAnim(13)\">⬅️ TURN LEFT</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(14)\">➡️ TURN RIGHT</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(15)\">⚠️ HAZARD</button>";
  html += "<button class=\"eye-btn\" onclick=\"setAnim(16)\">✏️ CUSTOM</button>";
  html += "</div>";
  html += "<div style=\"margin-top: 15px;\"><a href=\"/custom\"><button style=\"background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);\">🎨 Pixel Editor</button></a></div>";
  
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
  
  html += "<div class=\"slider-label\"><label>Turn Signal Duration (ms): <span id=\"sigDurVal\">3000</span></label></div>";
  html += "<input type=\"range\" min=\"1000\" max=\"10000\" step=\"500\" value=\"3000\" id=\"sigDur\" onchange=\"setSigDur(this.value)\" oninput=\"document.getElementById('sigDurVal').textContent=this.value\">";
  
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
function setSigDur(val) {
  fetch('/api/eyes/signal_duration?value=' + val).then(r => r.json()).then(d => console.log(d));
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
    const animNames = ['IDLE','BLINK','WINK LEFT','WINK RIGHT','LOOK LEFT','LOOK RIGHT','LOOK UP','LOOK DOWN','HAPPY','SAD','ANGRY','SURPRISED','SLEEP','TURN LEFT','TURN RIGHT','HAZARD','CUSTOM'];
    document.getElementById('currentAnim').textContent = animNames[d.animation] || 'UNKNOWN';
    document.getElementById('brightVal').textContent = d.brightness;
    document.getElementById('brightness').value = d.brightness;
    if (d.signalDuration) {
       document.getElementById('sigDur').value = d.signalDuration;
       document.getElementById('sigDurVal').textContent = d.signalDuration;
    }
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

String buildCustomEditorPage() {
  String html;
  html.reserve(8000);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>Pixel Editor - " + String(projectName()) + "</title>";
  html += "<style>";
  html += R"rawliteral(
body { font-family: 'Segoe UI', Tahoma, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); margin: 0; padding: 20px; }
.container { max-width: 800px; margin: 0 auto; }
h1 { color: white; text-align: center; }
.section { background: white; border-radius: 15px; padding: 25px; margin-bottom: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.2); }
.section-title { font-size: 1.5em; margin-bottom: 20px; color: #333; border-bottom: 2px solid #667eea; padding-bottom: 10px; }
.pixel-grid { display: inline-grid; grid-template-columns: repeat(8, 1fr); gap: 4px; padding: 20px; background: #f0f0f0; border-radius: 10px; }
.pixel { width: 40px; height: 40px; border: 2px solid #ccc; border-radius: 5px; background: #000; cursor: pointer; transition: transform 0.1s; }
.pixel:hover { transform: scale(1.1); }
.pixel.selected { border-color: #667eea; box-shadow: 0 0 8px #667eea; }
.controls { display: flex; gap: 15px; align-items: center; flex-wrap: wrap; margin-top: 20px; }
.color-input { display: flex; align-items: center; gap: 10px; }
.color-input label { font-weight: 600; color: #333; }
input[type="color"] { width: 60px; height: 40px; border: none; border-radius: 5px; cursor: pointer; }
.buttons { display: flex; gap: 10px; flex-wrap: wrap; }
button { background: #667eea; color: white; border: none; padding: 12px 20px; border-radius: 5px; cursor: pointer; font-weight: 600; font-size: 1em; transition: background 0.2s; }
button:hover { background: #5568d3; }
button.danger { background: #f44336; }
button.danger:hover { background: #da190b; }
button.success { background: #4caf50; }
button.success:hover { background: #388e3c; }
button.back { background: #999; }
button.back:hover { background: #777; }
.info { background: #e3f2fd; border-left: 4px solid #2196f3; padding: 15px; border-radius: 5px; margin-bottom: 20px; color: #1565c0; }
.instruction { font-size: 0.9em; margin-top: 10px; color: #666; }
)rawliteral";
  html += "</style></head><body>";
  html += "<div class=\"container\">";
  html += "<h1>✏️ Custom Pixel Editor</h1>";
  
  html += "<div class=\"section\">";
  html += "<div class=\"info\">Click on pixels to toggle them on/off. Choose a color, then click 'Apply Pattern' to send to device.</div>";
  
  html += "<div class=\"section-title\">Grid Editor (8x8)</div>";
  html += "<div class=\"pixel-grid\" id=\"pixelGrid\"></div>";
  
  html += "<div class=\"controls\">";
  html += "<div class=\"color-input\"><label>Color:</label><input type=\"color\" id=\"pixelColor\" value=\"#FF4500\"></div>";
  html += "</div>";
  
  html += "<div class=\"buttons\">";
  html += "<button class=\"success\" onclick=\"applyPattern()\">✓ Apply Pattern</button>";
  html += "<button class=\"danger\" onclick=\"clearGrid()\">🗑️ Clear All</button>";
  html += "<button onclick=\"randomize()\">🎲 Randomize</button>";
  html += "<button class=\"back\" onclick=\"location.href='/'\">← Back</button>";
  html += "</div>";
  html += "<div class=\"instruction\">• Off pixels = black (off)<br/>• On pixels = selected color (on)<br/>• Color changes apply only when you click 'Apply Pattern'</div>";
  html += "</div>";
  html += "</div>";
  
  html += R"rawliteral(<script>
const GRID_SIZE = 8;
const TOTAL_PIXELS = GRID_SIZE * GRID_SIZE;
let pixelStates = new Array(TOTAL_PIXELS).fill(0);

function initGrid() {
  const grid = document.getElementById('pixelGrid');
  grid.innerHTML = '';
  for (let i = 0; i < TOTAL_PIXELS; i++) {
    const pixel = document.createElement('div');
    pixel.className = 'pixel';
    pixel.id = 'pixel-' + i;
    pixel.onclick = () => togglePixel(i);
    if (pixelStates[i]) {
      pixel.style.background = document.getElementById('pixelColor').value;
      pixel.classList.add('selected');
    }
    grid.appendChild(pixel);
  }
}

function togglePixel(index) {
  pixelStates[index] = pixelStates[index] ? 0 : 1;
  const pixel = document.getElementById('pixel-' + index);
  if (pixelStates[index]) {
    pixel.style.background = document.getElementById('pixelColor').value;
    pixel.classList.add('selected');
  } else {
    pixel.style.background = '#000';
    pixel.classList.remove('selected');
  }
}

function applyPattern() {
  const color = document.getElementById('pixelColor').value;
  const rgb = parseInt(color.slice(1), 16);
  
  let requests = [];
  for (let i = 0; i < TOTAL_PIXELS; i++) {
    if (pixelStates[i]) {
      const x = i % 8;
      const y = Math.floor(i / 8);
      requests.push(
        fetch(`/api/eyes/custom/pixel?eye=left&x=${x}&y=${y}&color=${rgb}`)
          .then(r => r.json())
      );
      requests.push(
        fetch(`/api/eyes/custom/pixel?eye=right&x=${x}&y=${y}&color=${rgb}`)
          .then(r => r.json())
      );
    }
  }
  
  Promise.all(requests).then(results => {
    fetch('/api/eyes/animation?id=16').then(r => r.json()).then(d => {
      alert('Pattern applied!');
      console.log(d);
    });
  }).catch(e => {
    alert('Error applying pattern: ' + e);
    console.error(e);
  });
}

function clearGrid() {
  pixelStates = new Array(TOTAL_PIXELS).fill(0);
  initGrid();
  fetch('/api/eyes/custom/clear').then(r => r.json()).then(d => console.log(d));
}

function randomize() {
  pixelStates = pixelStates.map(() => Math.random() > 0.5 ? 1 : 0);
  initGrid();
}

document.getElementById('pixelColor').addEventListener('change', () => {
  document.querySelectorAll('.pixel.selected').forEach(pixel => {
    pixel.style.background = document.getElementById('pixelColor').value;
  });
});

initGrid();
</script></body></html>)rawliteral";
  
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
  json += "\"signalDuration\":" + String(NeoPixel::getSignalDuration()) + ",";
  json += "\"primary_color\":" + String(state.primaryColor) + ",";
  json += "\"secondary_color\":" + String(state.secondaryColor) + ",";
  json += "\"autoplay\":" + String(state.autoPlay ? "true" : "false");
  json += "}";
  return json;
  #else
  return "{\"animation\":0,\"brightness\":0,\"primary_color\":0,\"secondary_color\":0,\"autoplay\":false}";
  #endif
}

String buildCustomEditorPage() {
  String html;
  html.reserve(10000);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>Custom Pixel Editor</title>";
  html += "<style>";
  html += R"rawliteral(
body { font-family: 'Segoe UI', Tahoma, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); margin: 0; padding: 20px; }
.container { max-width: 1000px; margin: 0 auto; background: white; border-radius: 15px; padding: 30px; box-shadow: 0 10px 40px rgba(0,0,0,0.3); }
h1 { color: #667eea; text-align: center; margin-bottom: 10px; }
.subtitle { text-align: center; color: #666; margin-bottom: 30px; font-size: 0.9em; }
.controls { display: flex; gap: 20px; margin-bottom: 30px; flex-wrap: wrap; align-items: center; justify-content: center; }
.control-group { display: flex; flex-direction: column; gap: 8px; }
.control-group label { font-weight: 600; color: #333; font-size: 0.9em; }
.control-group input[type="color"] { width: 80px; height: 50px; border: 2px solid #ddd; border-radius: 8px; cursor: pointer; }
.grids-container { display: grid; grid-template-columns: 1fr 1fr; gap: 30px; margin-bottom: 30px; }
.grid-section { display: flex; flex-direction: column; align-items: center; }
.grid-title { font-weight: 700; font-size: 1.2em; margin-bottom: 15px; color: #333; }
.pixel-grid { display: grid; grid-template-columns: repeat(8, 50px); gap: 4px; background: #f0f0f0; padding: 15px; border-radius: 10px; }
.pixel { width: 50px; height: 50px; border: 2px solid #999; border-radius: 5px; background: #000; cursor: pointer; transition: transform 0.1s, box-shadow 0.1s; }
.pixel:hover { transform: scale(1.05); box-shadow: 0 0 10px rgba(102,126,234,0.5); }
.pixel.selected { box-shadow: 0 0 15px rgba(255,255,0,0.8); }
.buttons { display: flex; gap: 10px; justify-content: center; flex-wrap: wrap; margin-bottom: 20px; }
button { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 12px 25px; border-radius: 8px; cursor: pointer; font-weight: 600; font-size: 1em; transition: transform 0.2s; }
button:hover { transform: translateY(-2px); }
button:active { transform: translateY(0); }
button.clear { background: #e74c3c; }
button.apply { background: #27ae60; }
.message-box { background: #ecf0f1; border-left: 4px solid #667eea; padding: 15px; border-radius: 5px; min-height: 40px; font-size: 0.95em; color: #333; margin-bottom: 20px; display: flex; align-items: center; }
.message-box.success { border-left-color: #27ae60; background: #d5f4e6; color: #27ae60; }
.message-box.error { border-left-color: #e74c3c; background: #fadbd8; color: #e74c3c; }
.message-box.info { border-left-color: #667eea; background: #ebf5fb; color: #667eea; }
.info-text { background: #ecf0f1; border-left: 4px solid #667eea; padding: 15px; border-radius: 5px; margin-top: 20px; font-size: 0.9em; color: #333; }
  )rawliteral";
  html += "</style></head><body>";
  html += "<div class=\"container\">";
  html += "<h1>🎨 Custom Pixel Editor</h1>";
  html += "<p class=\"subtitle\">Design your own animation - separate patterns for left and right eye</p>";
  
  html += "<div class=\"controls\">";
  html += "<div class=\"control-group\">";
  html += "<label>Pixel Color</label>";
  html += "<input type=\"color\" id=\"pixelColor\" value=\"#FF4500\">";
  html += "</div>";
  html += "</div>";
  
  html += "<div class=\"message-box info\" id=\"messageBox\">💡 Select a color and click pixels to paint. Patterns are saved separately for each eye.</div>";
  
  html += "<div class=\"grids-container\">";
  html += "<div class=\"grid-section\">";
  html += "<div class=\"grid-title\">👁️ Left Eye</div>";
  html += "<div class=\"pixel-grid\" id=\"gridLeft\"></div>";
  html += "</div>";
  html += "<div class=\"grid-section\">";
  html += "<div class=\"grid-title\">👁️ Right Eye</div>";
  html += "<div class=\"pixel-grid\" id=\"gridRight\"></div>";
  html += "</div>";
  html += "</div>";
  
  html += "<div class=\"buttons\">";
  html += "<button class=\"clear\" onclick=\"clearBothGrids()\">🗑️ Clear All</button>";
  html += "<button class=\"clear\" onclick=\"clearGridLeft()\">🗑️ Clear Left</button>";
  html += "<button class=\"clear\" onclick=\"clearGridRight()\">🗑️ Clear Right</button>";
  html += "<button class=\"apply\" onclick=\"applyCustomPattern()\">✅ Apply & Activate</button>";
  html += "<button onclick=\"goHome()\">🏠 Back</button>";
  html += "</div>";
  
  html += "<div class=\"info-text\">";
  html += "💡 <strong>How to use:</strong> Select a color, paint pixels on both grids, then click Apply & Activate to set the custom animation.";
  html += "</div>";
  
  html += "</div>";
  html += "<script>";
  html += R"rawliteral(
const GRID_SIZE = 64;
let gridLeft = new Array(GRID_SIZE).fill(0);
let gridRight = new Array(GRID_SIZE).fill(0);

function initGrids() {
  loadPatterns();
  renderGrid('gridLeft', gridLeft);
  renderGrid('gridRight', gridRight);
}

function renderGrid(containerId, grid) {
  const container = document.getElementById(containerId);
  container.innerHTML = '';
  for (let i = 0; i < GRID_SIZE; i++) {
    const pixel = document.createElement('div');
    pixel.className = 'pixel';
    const color = grid[i] ? '#' + grid[i].toString(16).padStart(6, '0') : '#000000';
    pixel.style.backgroundColor = color;
    if (grid[i]) pixel.classList.add('selected');
    pixel.onclick = () => togglePixel(containerId, i, pixel, grid);
    container.appendChild(pixel);
  }
}

function togglePixel(gridId, index, element, gridArray) {
  const color = document.getElementById('pixelColor').value;
  gridArray[index] = gridArray[index] ? 0 : parseInt(color.replace('#', '0x'));
  const colorHex = gridArray[index] ? color : '#000000';
  element.style.backgroundColor = colorHex;
  element.classList.toggle('selected');
  savePatterns();
}

function clearGridLeft() {
  gridLeft.fill(0);
  renderGrid('gridLeft', gridLeft);
  savePatterns();
  showMessage('Left eye cleared', 'success');
}

function clearGridRight() {
  gridRight.fill(0);
  renderGrid('gridRight', gridRight);
  savePatterns();
  showMessage('Right eye cleared', 'success');
}

function clearBothGrids() {
  gridLeft.fill(0);
  gridRight.fill(0);
  renderGrid('gridLeft', gridLeft);
  renderGrid('gridRight', gridRight);
  savePatterns();
  showMessage('Both grids cleared', 'success');
}

function savePatterns() {
  localStorage.setItem('customPatternLeft', JSON.stringify(gridLeft));
  localStorage.setItem('customPatternRight', JSON.stringify(gridRight));
}

function loadPatterns() {
  const left = localStorage.getItem('customPatternLeft');
  const right = localStorage.getItem('customPatternRight');
  if (left) gridLeft = JSON.parse(left);
  if (right) gridRight = JSON.parse(right);
}

function applyCustomPattern() {
  const patternLeftStr = gridLeft.map(c => c.toString()).join(',');
  const patternRightStr = gridRight.map(c => c.toString()).join(',');
  
  fetch(`/api/custom/apply?eye=both&pattern_left=${patternLeftStr}&pattern_right=${patternRightStr}`)
    .then(r => r.json())
    .then(d => {
      if (d.status === 'ok') {
        showMessage('✅ Custom patterns applied and activated!', 'success');
        setTimeout(() => window.location.href = '/', 2000);
      } else {
        showMessage('❌ Error: ' + d.message, 'error');
      }
    })
    .catch(e => showMessage('❌ Error: ' + e, 'error'));
}

function showMessage(text, type) {
  const box = document.getElementById('messageBox');
  box.textContent = text;
  box.className = 'message-box ' + type;
}

function goHome() {
  window.location.href = '/';
}

initGrids();
  )rawliteral";
  html += "</script>";
  html += "</body></html>";
  
  return html;
}

}  // namespace WebUI
