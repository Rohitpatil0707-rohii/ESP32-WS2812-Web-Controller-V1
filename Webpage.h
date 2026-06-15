#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 LED Control</title>

  <style>
    * { box-sizing: border-box; }

    body {
      margin: 0;
      font-family: Arial, Helvetica, sans-serif;
      background: radial-gradient(circle at top, #0f172a, #020617 70%);
      color: white;
      text-align: center;
    }

    .container {
      max-width: 520px;
      margin: 0 auto;
      padding: 20px 15px 25px;
    }

    .top-row {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 8px;
      margin-top: 8px;
    }

    .wifi-icon {
      font-size: 28px;
      color: #22d3ee;
      text-shadow: 0 0 18px #22d3ee;
      font-weight: bold;
    }

    .strip-icon {
      font-size: 28px;
      color: #facc15;
      text-shadow: 0 0 18px #facc15;
      font-weight: bold;
    }

    .status-block {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 7px;
      padding: 8px 12px;
      border-radius: 999px;
      font-size: 12px;
      font-weight: 900;
      background: rgba(15, 23, 42, 0.9);
      border: 1px solid rgba(148, 163, 184, 0.35);
      box-shadow: 0 0 14px rgba(148, 163, 184, 0.18);
      min-width: 105px;
    }

    .status-dot {
      width: 10px;
      height: 10px;
      border-radius: 50%;
      background: #ef4444;
      box-shadow: 0 0 10px #ef4444;
    }

    .status-online {
      color: #86efac;
      border-color: rgba(34, 197, 94, 0.45);
      box-shadow: 0 0 14px rgba(34, 197, 94, 0.35);
    }

    .status-online .status-dot {
      background: #22c55e;
      box-shadow: 0 0 12px #22c55e;
    }

    .status-offline {
      color: #fca5a5;
      border-color: rgba(239, 68, 68, 0.45);
      box-shadow: 0 0 14px rgba(239, 68, 68, 0.35);
    }

    .status-offline .status-dot {
      background: #ef4444;
      box-shadow: 0 0 12px #ef4444;
    }

    h1 {
      margin: 15px 0 8px;
      font-size: 34px;
      font-weight: 900;
      letter-spacing: 1px;
      background: linear-gradient(90deg, #ff00ff, #ff8800, #ffff00, #00ff66, #00ccff);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    .login-status {
      display: inline-block;
      margin-bottom: 14px;
      padding: 7px 14px;
      border-radius: 999px;
      font-size: 13px;
      font-weight: bold;
      color: #a7f3d0;
      background: rgba(20, 184, 166, 0.14);
      border: 1px solid rgba(20, 184, 166, 0.35);
      box-shadow: 0 0 14px rgba(20, 184, 166, 0.22);
    }

    .section-title {
      display: flex;
      align-items: center;
      gap: 12px;
      margin: 18px 0 16px;
      font-size: 22px;
      font-weight: 900;
      letter-spacing: 1px;
    }

    .section-title::before,
    .section-title::after {
      content: "";
      flex: 1;
      height: 3px;
      border-radius: 5px;
      background: linear-gradient(90deg, transparent, #22d3ee, transparent);
      box-shadow: 0 0 10px #22d3ee;
    }

    .panel,
    .control-panel {
      border: 1px solid rgba(148,163,184,0.25);
      border-radius: 22px;
      padding: 14px;
      background: rgba(2,6,23,0.35);
      box-shadow: inset 0 0 20px rgba(148,163,184,0.08);
      margin-top: 18px;
    }

    .control-title {
      font-size: 20px;
      font-weight: 900;
      margin-bottom: 12px;
      color: #e2e8f0;
    }

    .color-grid,
    .mode-grid {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      gap: 12px;
    }

    .speed-grid,
    .bottom-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
    }

    button {
      width: 100%;
      border: none;
      border-radius: 16px;
      padding: 18px 8px;
      color: white;
      font-size: 14px;
      font-weight: 900;
      cursor: pointer;
      box-shadow: 0 0 18px rgba(255,255,255,0.12);
    }

    button:active {
      transform: scale(0.96);
    }

    .icon {
      display: block;
      font-size: 22px;
      margin-bottom: 7px;
    }

    .red { background: linear-gradient(145deg, #ff1f1f, #8b0000); box-shadow: 0 0 16px #ef4444; }
    .green { background: linear-gradient(145deg, #22ff22, #006b12); box-shadow: 0 0 16px #22c55e; }
    .blue { background: linear-gradient(145deg, #2589ff, #003a9f); box-shadow: 0 0 16px #3b82f6; }
    .white { background: linear-gradient(145deg, #ffffff, #9ca3af); color: #111827; box-shadow: 0 0 16px #e5e7eb; }
    .yellow { background: linear-gradient(145deg, #ffe600, #b77900); color: #111827; box-shadow: 0 0 16px #eab308; }
    .cyan { background: linear-gradient(145deg, #22f7ff, #007b8a); box-shadow: 0 0 16px #06b6d4; }
    .magenta { background: linear-gradient(145deg, #ff20df, #8b006f); box-shadow: 0 0 16px #d946ef; }
    .orange { background: linear-gradient(145deg, #ff9f1c, #b34700); box-shadow: 0 0 16px #f97316; }
    .purple { background: linear-gradient(145deg, #a855f7, #4c1d95); box-shadow: 0 0 16px #8b5cf6; }

    .mode {
      background: linear-gradient(145deg, #334155, #0f172a);
      box-shadow: 0 0 16px rgba(34,211,238,0.4);
    }

    .rainbow {
      background: linear-gradient(90deg, red, orange, yellow, green, cyan, blue, violet);
      box-shadow: 0 0 22px #d946ef;
    }

    .off {
      background: linear-gradient(145deg, #1e293b, #020617);
      box-shadow: 0 0 16px rgba(148,163,184,0.35);
    }

    .fire {
      background: linear-gradient(145deg, #ff4500, #7f1d1d);
      box-shadow: 0 0 16px #f97316;
    }

    .picker-box {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 15px;
      margin-bottom: 12px;
    }

    input[type="color"] {
      width: 90px;
      height: 55px;
      border: none;
      border-radius: 12px;
      background: transparent;
    }

    input[type="range"] {
      width: 100%;
    }

    .speed-value,
    .brightness-value {
      font-size: 16px;
      font-weight: bold;
      color: #22d3ee;
      margin-bottom: 14px;
    }

    .speed {
      background: linear-gradient(145deg, #14b8a6, #0f766e);
      box-shadow: 0 0 16px #14b8a6;
    }

    .blink {
      background: linear-gradient(145deg, #8b5cf6, #4c1d95);
      box-shadow: 0 0 16px #8b5cf6;
    }

    .settings {
      background: linear-gradient(145deg, #64748b, #1e293b);
      box-shadow: 0 0 16px rgba(148,163,184,0.4);
      margin-top: 12px;
    }

    .logout {
      background: linear-gradient(145deg, #ef4444, #7f1d1d);
      box-shadow: 0 0 16px rgba(239,68,68,0.75);
      margin-top: 12px;
    }

    .footer {
      margin-top: 16px;
      font-size: 14px;
      color: #cbd5e1;
      line-height: 1.5;
    }

    .footer strong {
      color: #ffffff;
    }

    .handle {
      font-family: "Courier New", monospace;
      font-size: 17px;
      font-weight: 900;
      letter-spacing: 2px;
      background: linear-gradient(90deg, #ff00ff, #ff8800, #ffff00, #00ff66, #00ccff, #8b5cf6);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    @media (max-width: 430px) {
      h1 { font-size: 28px; }
      .wifi-icon, .strip-icon { font-size: 22px; }
      .status-block { font-size: 10px; padding: 7px 9px; min-width: 90px; }
      .color-grid, .mode-grid { gap: 10px; }
      button { font-size: 12px; padding: 16px 5px; }
      .footer { font-size: 13px; }
      .handle { font-size: 16px; }
    }
  </style>
</head>

<body>
  <div class="container">

    <div class="top-row">
      <div class="wifi-icon">WiFi</div>

      <div id="deviceStatus" class="status-block status-offline">
        <span class="status-dot"></span>
        <span id="statusText">OFFLINE</span>
      </div>

      <div class="strip-icon">LED</div>
    </div>

    <h1>ESP32 LED CONTROL</h1>
    <div class="login-status">SECURE LOGIN ACTIVE</div>

    <div class="section-title">COLORS</div>

    <div class="panel">
      <div class="color-grid">
        <button class="red" onclick="sendCommand('/red')"><span class="icon">*</span>RED</button>
        <button class="green" onclick="sendCommand('/green')"><span class="icon">*</span>GREEN</button>
        <button class="blue" onclick="sendCommand('/blue')"><span class="icon">*</span>BLUE</button>

        <button class="white" onclick="sendCommand('/on')"><span class="icon">*</span>WHITE</button>
        <button class="yellow" onclick="sendCommand('/yellow')"><span class="icon">*</span>YELLOW</button>
        <button class="cyan" onclick="sendCommand('/cyan')"><span class="icon">*</span>CYAN</button>

        <button class="magenta" onclick="sendCommand('/magenta')"><span class="icon">*</span>MAGENTA</button>
        <button class="orange" onclick="sendCommand('/orange')"><span class="icon">*</span>ORANGE</button>
        <button class="purple" onclick="sendCommand('/purple')"><span class="icon">*</span>PURPLE</button>
      </div>
    </div>

    <div class="control-panel">
      <div class="control-title">CUSTOM COLOR</div>

      <div class="picker-box">
        <input type="color" id="colorPicker" value="#ff0000" onchange="sendColor()">
        <button class="rainbow" onclick="sendColor()">SET COLOR</button>
      </div>
    </div>

    <div class="control-panel">
      <div class="control-title">BRIGHTNESS</div>
      <div class="brightness-value" id="brightnessValue">Brightness: 120</div>
      <input type="range" id="brightnessSlider" min="0" max="255" value="120" oninput="sendBrightness(this.value)">
    </div>

    <div class="section-title">BASIC ANIMATION</div>

    <div class="control-panel">
      <div class="mode-grid">
        <button class="mode" onclick="sendCommand('/solid')">SOLID</button>
        <button class="blink" onclick="sendCommand('/blink')">BLINK</button>
        <button class="rainbow" onclick="sendCommand('/rainbow')">RAINBOW</button>

        <button class="mode" onclick="sendCommand('/breathing')">BREATHING</button>
        <button class="mode" onclick="sendCommand('/colorwipe')">COLOR WIPE</button>
        <button class="mode" onclick="sendCommand('/theater')">THEATER</button>

        <button class="mode" onclick="sendCommand('/sparkle')">SPARKLE</button>
        <button class="mode" onclick="sendCommand('/police')">POLICE</button>
        <button class="off" onclick="sendCommand('/off')">OFF</button>
      </div>
    </div>

    <div class="section-title">EXTRA ANIMATION</div>

    <div class="control-panel">
      <div class="mode-grid">
        <button class="mode" onclick="sendCommand('/running')">RUNNING</button>
        <button class="mode" onclick="sendCommand('/comet')">COMET</button>
        <button class="mode" onclick="sendCommand('/meteor')">METEOR</button>

        <button class="fire" onclick="sendCommand('/fire')">FIRE</button>
        <button class="mode" onclick="sendCommand('/twinkle')">TWINKLE</button>
        <button class="mode" onclick="sendCommand('/scanner')">SCANNER</button>

        <button class="blink" onclick="sendCommand('/strobe')">STROBE</button>
        <button class="mode" onclick="sendCommand('/randomcolor')">RANDOM</button>
        <button class="mode" onclick="sendCommand('/rgbfade')">RGB FADE</button>

        <button class="rainbow" onclick="sendCommand('/rainbowcycle')">RAINBOW CYCLE</button>
        <button class="mode" onclick="sendCommand('/colorbounce')">BOUNCE</button>
        <button class="mode" onclick="sendCommand('/snake')">SNAKE</button>
      </div>
    </div>

    <div class="control-panel">
      <div class="control-title">SPEED CONTROL</div>
      <div class="speed-value" id="speedValue">Speed: 300 ms</div>

      <div class="speed-grid">
        <button class="speed" onclick="sendCommand('/speeddown')">SPEED -</button>
        <button class="speed" onclick="sendCommand('/speedup')">SPEED +</button>

        <button class="blink" onclick="sendCommand('/blinkslow')">SLOW BLINK</button>
        <button class="blink" onclick="sendCommand('/blinkfast')">FAST BLINK</button>
      </div>
    </div>

    <div class="bottom-grid">
      <button class="settings" onclick="openSettings()">SETTINGS</button>
      <button class="logout" onclick="logoutUser()">LOGOUT</button>
    </div>

    <div class="footer">
      Made with ❤️ by <strong>Rohit Patil</strong><br>
      <span class="handle">only_kabaddi_07</span>
    </div>

  </div>

  <script>
    function updateDeviceStatus(isOnline) {
      let block = document.getElementById("deviceStatus");
      let text = document.getElementById("statusText");

      if (isOnline) {
        block.className = "status-block status-online";
        text.innerHTML = "ONLINE";
      } else {
        block.className = "status-block status-offline";
        text.innerHTML = "OFFLINE";
      }
    }

    function checkDeviceOnline() {
      fetch("/ping", { cache: "no-store" })
        .then(response => {
          if (response.redirected) {
            window.location.href = response.url;
            return "";
          }

          return response.text();
        })
        .then(data => {
          if (data === "pong") {
            updateDeviceStatus(true);
          } else {
            updateDeviceStatus(false);
          }
        })
        .catch(error => {
          updateDeviceStatus(false);
        });
    }

    function sendCommand(url) {
      fetch(url)
        .then(response => {
          if (response.redirected) {
            window.location.href = response.url;
            return "";
          }

          if (response.status == 401 || response.status == 403) {
            window.location.href = "/login";
            return "";
          }

          return response.text();
        })
        .then(data => {
          if (data.includes("ms")) {
            document.getElementById("speedValue").innerHTML = data;
          }
        })
        .catch(error => {
          console.log("Command failed");
          updateDeviceStatus(false);
        });
    }

    function sendColor() {
      let color = document.getElementById("colorPicker").value;

      let r = parseInt(color.substring(1, 3), 16);
      let g = parseInt(color.substring(3, 5), 16);
      let b = parseInt(color.substring(5, 7), 16);

      fetch("/color?r=" + r + "&g=" + g + "&b=" + b)
        .then(response => {
          if (response.redirected) {
            window.location.href = response.url;
          }
        })
        .catch(error => {
          console.log("Color command failed");
          updateDeviceStatus(false);
        });
    }

    function sendBrightness(value) {
      document.getElementById("brightnessValue").innerHTML = "Brightness: " + value;

      fetch("/brightness?value=" + value)
        .then(response => {
          if (response.redirected) {
            window.location.href = response.url;
          }
        })
        .catch(error => {
          console.log("Brightness command failed");
          updateDeviceStatus(false);
        });
    }

    function openSettings() {
      window.location.href = "/settings";
    }

    function logoutUser() {
      window.location.href = "/logout";
    }

    window.onload = function() {
      checkDeviceOnline();
      setInterval(checkDeviceOnline, 2000);

      fetch('/speed')
        .then(response => {
          if (response.redirected) {
            window.location.href = response.url;
            return "";
          }
          return response.text();
        })
        .then(data => {
          if (data.length > 0) {
            document.getElementById("speedValue").innerHTML = "Speed: " + data;
          }
        })
        .catch(error => {
          updateDeviceStatus(false);
        });

      fetch('/brightnessstatus')
        .then(response => {
          if (response.redirected) {
            window.location.href = response.url;
            return "";
          }
          return response.text();
        })
        .then(data => {
          if (data.length > 0) {
            document.getElementById("brightnessValue").innerHTML = "Brightness: " + data;
            document.getElementById("brightnessSlider").value = data;
          }
        })
        .catch(error => {
          updateDeviceStatus(false);
        });
    };
  </script>

</body>
</html>
)rawliteral";

#endif