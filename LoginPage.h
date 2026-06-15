#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

const char LOGIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 LED Login</title>

  <style>
    * {
      box-sizing: border-box;
    }

    body {
      margin: 0;
      min-height: 100vh;
      font-family: Arial, Helvetica, sans-serif;
      background: radial-gradient(circle at top, #0f172a, #020617 70%);
      color: white;
      display: flex;
      align-items: center;
      justify-content: center;
      text-align: center;
    }

    .container {
      width: 100%;
      max-width: 430px;
      padding: 20px;
    }

    .logo {
      width: 82px;
      height: 82px;
      margin: 0 auto 14px;
      border-radius: 50%;
      background: conic-gradient(#ff004c, #ff8800, #ffff00, #00ff66, #00ccff, #7c3aed, #ff004c);
      box-shadow: 0 0 28px rgba(56,189,248,0.6);
      display: flex;
      align-items: center;
      justify-content: center;
    }

    .logo-inner {
      width: 58px;
      height: 58px;
      border-radius: 50%;
      background: #020617;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 20px;
      font-weight: 900;
      color: #22d3ee;
      letter-spacing: 1px;
    }

    h1 {
      margin: 8px 0 6px;
      font-size: 34px;
      font-weight: 900;
      background: linear-gradient(90deg, #ff00ff, #ff8800, #ffff00, #00ff66, #00ccff);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    .subtitle {
      color: #cbd5e1;
      font-size: 15px;
      margin-bottom: 12px;
      font-weight: bold;
    }

    .status-block {
      width: fit-content;
      margin: 0 auto 20px;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 7px;
      padding: 8px 14px;
      border-radius: 999px;
      font-size: 12px;
      font-weight: 900;
      background: rgba(15, 23, 42, 0.9);
      border: 1px solid rgba(148, 163, 184, 0.35);
      box-shadow: 0 0 14px rgba(148, 163, 184, 0.18);
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

    .card {
      background: rgba(15, 23, 42, 0.92);
      border: 1px solid rgba(56, 189, 248, 0.28);
      border-radius: 24px;
      padding: 22px;
      box-shadow: 0 0 28px rgba(56, 189, 248, 0.16);
      text-align: left;
    }

    label {
      display: block;
      margin-top: 15px;
      margin-bottom: 7px;
      color: #22d3ee;
      font-weight: bold;
      font-size: 14px;
    }

    input {
      width: 100%;
      padding: 15px;
      border-radius: 14px;
      border: 1px solid #334155;
      background: #020617;
      color: white;
      font-size: 17px;
      outline: none;
    }

    input:focus {
      border-color: #22d3ee;
      box-shadow: 0 0 12px rgba(34, 211, 238, 0.45);
    }

    .password-box {
      position: relative;
    }

    .eye {
      position: absolute;
      right: 10px;
      top: 50%;
      transform: translateY(-50%);
      width: auto;
      padding: 8px 10px;
      border-radius: 10px;
      background: rgba(34, 211, 238, 0.12);
      color: #22d3ee;
      border: 1px solid rgba(34, 211, 238, 0.35);
      font-size: 12px;
      font-weight: 900;
      cursor: pointer;
      box-shadow: none;
    }

    .login-btn {
      width: 100%;
      margin-top: 22px;
      border: none;
      border-radius: 18px;
      padding: 17px;
      color: white;
      font-size: 17px;
      font-weight: 900;
      cursor: pointer;
      background: linear-gradient(145deg, #14b8a6, #0f766e);
      box-shadow: 0 0 18px rgba(20,184,166,0.75);
    }

    .login-btn:active {
      transform: scale(0.98);
    }

    .error {
      margin-top: 14px;
      padding: 12px;
      border-radius: 14px;
      background: rgba(239, 68, 68, 0.15);
      color: #fca5a5;
      border: 1px solid rgba(239, 68, 68, 0.35);
      font-weight: bold;
      text-align: center;
      display: none;
    }

    .footer {
      margin-top: 18px;
      color: #cbd5e1;
      font-size: 14px;
      text-align: center;
    }

    .handle {
      font-family: "Courier New", monospace;
      font-size: 16px;
      font-weight: 900;
      letter-spacing: 2px;
      background: linear-gradient(90deg, #ff00ff, #ff8800, #ffff00, #00ff66, #00ccff, #8b5cf6);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    @media (max-width: 430px) {
      h1 {
        font-size: 30px;
      }

      .container {
        padding: 16px;
      }

      .card {
        padding: 18px;
      }
    }
  </style>
</head>

<body>
  <div class="container">

    <div class="logo">
      <div class="logo-inner">💡</div>
    </div>

    <h1>LED LOGIN</h1>
    <div class="subtitle">Secure ESP32 LED Controller</div>

    <div id="deviceStatus" class="status-block status-offline">
      <span class="status-dot"></span>
      <span id="statusText">OFFLINE</span>
    </div>

    <div class="card">
      <form action="/logincheck" method="POST">

        <label>Username</label>
        <input type="text" name="username" placeholder="Enter username" required>

        <label>Password</label>
        <div class="password-box">
          <input id="password" type="password" name="password" placeholder="Enter password" required>
          <button class="eye" type="button" onclick="togglePassword()">👁</button>
        </div>

        <button class="login-btn" type="submit">LOGIN</button>
      </form>

      <div id="errorBox" class="error">
        Wrong username or password
      </div>
    </div>

    <div class="footer">
      Made with Love by <b>Rohit Patil</b><br>
      <span class="handle">only_kabaddi_07</span>
    </div>
  </div>

  <script>
    function togglePassword() {
      let p = document.getElementById("password");
      let btn = document.querySelector(".eye");

      if (p.type === "password") {
        p.type = "text";
        btn.innerHTML = "HIDE";
      } else {
        p.type = "password";
        btn.innerHTML = "SHOW";
      }
    }

    function updateDeviceStatus(isOnline) {
      let block = document.getElementById("deviceStatus");
      let text = document.getElementById("statusText");

      if (isOnline) {
        block.className = "status-block status-online";
        text.innerHTML = "DEVICE ONLINE";
      } else {
        block.className = "status-block status-offline";
        text.innerHTML = "DEVICE OFFLINE";
      }
    }

    function checkDeviceOnline() {
      fetch("/ping", { cache: "no-store" })
        .then(response => response.text())
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

    const params = new URLSearchParams(window.location.search);

    if (params.get("error") === "1") {
      document.getElementById("errorBox").style.display = "block";
    }

    window.onload = function() {
      checkDeviceOnline();
      setInterval(checkDeviceOnline, 2000);
    };
  </script>

</body>
</html>
)rawliteral";

#endif