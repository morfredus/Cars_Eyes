#include "web_styles.h"

namespace WebStyles {

const char* getDashboardStyles() {
  return R"(
    body {
      font-family: Inter, Arial, sans-serif;
      background: linear-gradient(135deg, #0f172a, #1e293b);
      color: #e2e8f0;
      margin: 0;
      padding: 24px;
    }
    .shell {
      max-width: 960px;
      margin: 0 auto;
    }
    h1 {
      margin: 0 0 12px;
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
      gap: 12px;
    }
    .card {
      background: #0b1220;
      border: 1px solid #243148;
      border-radius: 12px;
      padding: 16px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.35);
    }
    .badge {
      display: inline-block;
      padding: 4px 8px;
      border-radius: 999px;
      background: #10b981;
      color: #042f2e;
      font-weight: 700;
      font-size: 12px;
    }
    button {
      background: #2563eb;
      color: white;
      border: none;
      border-radius: 10px;
      padding: 12px 16px;
      font-size: 15px;
      cursor: pointer;
    }
    button:hover {
      background: #1d4ed8;
    }
    button.active {
      background: #10b981;
    }
    button.active:hover {
      background: #059669;
    }
    .section {
      background: #111827;
      border: 1px solid #243148;
      border-radius: 12px;
      padding: 16px;
      margin-bottom: 16px;
    }
    .section-title {
      font-size: 16px;
      font-weight: 600;
      margin-bottom: 12px;
      color: #f1f5f9;
    }
    .eye-controls, .color-scheme-group {
      display: flex;
      flex-wrap: wrap;
      gap: 8px;
      margin-bottom: 16px;
    }
    .eye-btn, .scheme-btn {
      flex: 1;
      min-width: 100px;
      padding: 10px 12px;
      font-size: 13px;
      background: #1e3a5f;
      color: #e2e8f0;
      border: 2px solid #243148;
    }
    .eye-btn:hover, .scheme-btn:hover {
      background: #2d5a9f;
      border-color: #60a5fa;
    }
    .color-picker-group {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
      gap: 12px;
      margin-bottom: 16px;
    }
    .color-group {
      display: flex;
      flex-direction: column;
      gap: 6px;
    }
    .color-group label {
      font-size: 13px;
      font-weight: 500;
    }
    .color-group input[type="color"] {
      height: 40px;
      border: none;
      border-radius: 6px;
      cursor: pointer;
    }
    .slider-label {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 8px;
      font-size: 14px;
    }
    input[type="range"] {
      width: 100%;
      cursor: pointer;
      margin-bottom: 16px;
    }
    .switch {
      position: relative;
      display: inline-block;
      width: 50px;
      height: 24px;
    }
    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      transition: .4s;
      border-radius: 24px;
    }
    .slider:before {
      position: absolute;
      content: "";
      height: 18px;
      width: 18px;
      left: 3px;
      bottom: 3px;
      background-color: white;
      transition: .4s;
      border-radius: 50%;
    }
    input:checked + .slider {
      background-color: #10b981;
    }
    input:checked + .slider:before {
      transform: translateX(26px);
    }
    a {
      color: #60a5fa;
      text-decoration: none;
    }
    a:hover {
      text-decoration: underline;
    }
  )";
}

const char* getOtaStyles() {
  return R"(
    body {
      font-family: Inter, Arial, sans-serif;
      background: #0b1220;
      color: #e2e8f0;
      margin: 0;
      padding: 24px;
    }
    .card {
      max-width: 520px;
      margin: 0 auto;
      background: #111827;
      border: 1px solid #1f2937;
      border-radius: 12px;
      padding: 20px;
      box-shadow: 0 12px 30px rgba(0, 0, 0, 0.35);
    }
    h2 {
      margin-top: 0;
      color: #e2e8f0;
    }
    button, input[type=submit] {
      background: #2563eb;
      color: white;
      border: none;
      border-radius: 10px;
      padding: 12px 16px;
      font-size: 15px;
      cursor: pointer;
      width: 100%;
      box-sizing: border-box;
    }
    button:hover, input[type=submit]:hover {
      background: #1d4ed8;
    }
    input[type=file] {
      display: block;
      margin-bottom: 12px;
      padding: 8px;
      background: #1f2937;
      border: 1px solid #374151;
      border-radius: 6px;
      color: #e2e8f0;
      width: 100%;
      box-sizing: border-box;
    }
    .progress {
      width: 100%;
      background: #1f2937;
      border-radius: 10px;
      overflow: hidden;
      margin: 12px 0;
      height: 14px;
    }
    .bar {
      width: 0;
      height: 100%;
      background: #10b981;
      transition: width 0.15s ease;
    }
    #status {
      font-size: 14px;
      margin-top: 8px;
      color: #cbd5e1;
      text-align: center;
    }
    p {
      text-align: center;
    }
    a {
      text-decoration: none;
    }
  )";
}

const char* getOtaScript() {
  return R"(
    const f = document.getElementById('otaForm');
    const bar = document.getElementById('bar');
    const st = document.getElementById('status');
    
    f.addEventListener('submit', e => {
      e.preventDefault();
      const fd = new FormData(f);
      const xhr = new XMLHttpRequest();
      xhr.open('POST', '/update');
      xhr.upload.onprogress = e => {
        if (e.lengthComputable) {
          const p = Math.round((e.loaded / e.total) * 100);
          bar.style.width = p + '%';
          st.textContent = 'Uploading ' + p + '%';
        }
      };
      xhr.onload = () => {
        if (xhr.status == 200) {
          bar.style.width = '100%';
          st.textContent = 'Upload OK, rebooting...';
          setTimeout(() => location.href = '/', 4000);
        } else {
          st.textContent = 'Error ' + xhr.status;
        }
      };
      xhr.onerror = () => st.textContent = 'Upload failed';
      xhr.send(fd);
    });
  )";
}

}  // namespace WebStyles
