#ifndef CONFIG_PAGE_H
#define CONFIG_PAGE_H

const char CONFIG_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Setup</title>

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
      max-width: 460px;
      margin: 0 auto;
      padding: 25px 16px;
    }

    h1 {
      margin: 10px 0 8px;
      font-size: 32px;
      font-weight: 900;
      background: linear-gradient(90deg, #ff00ff, #ff8800, #ffff00, #00ff66, #00ccff);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    .subtitle {
      color: #cbd5e1;
      margin-bottom: 22px;
      font-weight: bold;
    }

    .card {
      background: rgba(15,23,42,0.88);
      border: 1px solid rgba(56,189,248,0.25);
      border-radius: 22px;
      padding: 18px;
      box-shadow: 0 0 20px rgba(56,189,248,0.15);
      text-align: left;
    }

    label {
      display: block;
      margin-top: 14px;
      margin-bottom: 6px;
      color: #22d3ee;
      font-weight: bold;
      font-size: 14px;
    }

    input {
      width: 100%;
      padding: 14px;
      border-radius: 12px;
      border: 1px solid #334155;
      background: #020617;
      color: white;
      font-size: 16px;
      outline: none;
    }

    input:focus {
      border-color: #22d3ee;
      box-shadow: 0 0 10px rgba(34,211,238,0.4);
    }

    button {
      width: 100%;
      margin-top: 18px;
      border: none;
      border-radius: 16px;
      padding: 16px;
      color: white;
      font-size: 16px;
      font-weight: 900;
      cursor: pointer;
    }

    .save {
      background: linear-gradient(145deg, #14b8a6, #0f766e);
      box-shadow: 0 0 16px #14b8a6;
    }

    .reset {
      background: linear-gradient(145deg, #ef4444, #7f1d1d);
      box-shadow: 0 0 16px #ef4444;
    }

    .info {
      margin-top: 16px;
      color: #94a3b8;
      font-size: 13px;
      line-height: 1.5;
      text-align: center;
    }

    .warning {
      color: #facc15;
      font-size: 12px;
      line-height: 1.5;
      margin-top: 5px;
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
  </style>
</head>

<body>
  <div class="container">
    <h1>ESP32 SETUP</h1>
    <div class="subtitle">Wi-Fi, Device & Security</div>

    <div class="card">
      <form action="/saveconfig" method="POST">

        <label>Wi-Fi SSID</label>
        <input type="text" name="ssid" value="%SSID%" placeholder="Enter Wi-Fi name" required>

        <label>Wi-Fi Password</label>
        <input type="password" name="pass" placeholder="Leave blank to keep old password">
        <div class="warning">For new Wi-Fi, enter password. For old Wi-Fi, leave blank to keep saved password.</div>

        <label>Device Name</label>
        <input type="text" name="devname" value="%DEVNAME%" placeholder="esp32-led" required>

        <label>OTA Password</label>
        <input type="password" name="otapass" placeholder="Leave blank to keep old OTA password">
        <div class="warning">Used for Arduino OTA upload.</div>

        <label>Admin Username</label>
        <input type="text" name="adminuser" value="%ADMINUSER%" placeholder="admin" required>

        <label>Admin Password</label>
        <input type="password" name="adminpass" placeholder="Leave blank to keep old admin password">
        <div class="warning">This password protects LED page, settings page, and control commands.</div>

        <button class="save" type="submit">SAVE & RESTART</button>
      </form>

      <form action="/resetconfig" method="POST">
        <button class="reset" type="submit">RESET CONFIG</button>
      </form>

      <div class="info">
        First time setup hotspot:<br>
        <b>ESP32_LED_SETUP</b><br><br>
        Password:<br>
        <b>12345678</b><br><br>
        Setup page:<br>
        <b>192.168.4.1</b><br><br>
        Default login:<br>
        <b>admin / change_me_admin</b>
      </div>
    </div>

    <div class="footer">
      Made with LOVE by <b>Rohit Patil</b><br>
      <span class="handle">only_kabaddi_07</span>
    </div>
  </div>
</body>
</html>
)rawliteral";

#endif