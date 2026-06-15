#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "Auth.h"
#include "Config.h"
#include "ConfigPage.h"
#include "LED.h"

Preferences configPrefs;

String savedSSID = "";
String savedPASS = "";
String savedDeviceName = "esp32-led";
String savedOtaPassword = "change_me_ota";

String savedAdminUser = "admin";
String savedAdminPassword = "change_me_admin";

bool configMode = false;

void loadConfig() {
  savedSSID = configPrefs.getString("ssid", "");
  savedPASS = configPrefs.getString("pass", "");
  savedDeviceName = configPrefs.getString("devname", "esp32-led");
  savedOtaPassword = configPrefs.getString("otapass", "12345678");

  savedAdminUser = configPrefs.getString("adminuser", "admin");
  savedAdminPassword = configPrefs.getString("adminpass", "admin12345");

  if (savedAdminUser.length() == 0) {
    savedAdminUser = "admin";
  }

  if (savedAdminPassword.length() == 0) {
    savedAdminPassword = "admin12345";
  }
}

void configBegin() {
  configPrefs.begin("wifi_config", false);
  loadConfig();

  Serial.println("Config loaded");
  Serial.print("Saved SSID: ");
  Serial.println(savedSSID);
  Serial.print("Device Name: ");
  Serial.println(savedDeviceName);
  Serial.print("Admin User: ");
  Serial.println(savedAdminUser);
}

String getDeviceName() {
  return savedDeviceName;
}

String getOtaPassword() {
  return savedOtaPassword;
}

String getAdminUser() {
  return savedAdminUser;
}

String getAdminPassword() {
  return savedAdminPassword;
}

bool isConfigMode() {
  return configMode;
}

bool requireAuthentication(WebServer &server) {
  if (configMode) {
    return true;
  }

  if (server.authenticate(savedAdminUser.c_str(), savedAdminPassword.c_str())) {
    return true;
  }

  server.requestAuthentication();
  return false;
}

String getConfigPageHtml() {
  String page = FPSTR(CONFIG_PAGE);

  page.replace("%SSID%", savedSSID);
  page.replace("%DEVNAME%", savedDeviceName);
  page.replace("%ADMINUSER%", savedAdminUser);

  return page;
}

void startSetupAP() {
  WiFi.mode(WIFI_AP);

  bool result = WiFi.softAP("ESP32_LED_SETUP", "change123");

  if (result) {
    Serial.println("Setup AP started");
    Serial.println("Hotspot Name: ESP32_LED_SETUP");
    Serial.println("Hotspot Password: 12345678");
    Serial.print("Setup IP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start Setup AP");
  }

  configMode = true;
}

bool connectWiFiFromConfig(unsigned long timeoutMs) {
  if (savedSSID.length() == 0) {
    Serial.println("No saved Wi-Fi. Starting setup AP...");
    startSetupAP();
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(savedSSID.c_str(), savedPASS.c_str());

  Serial.print("Connecting to saved Wi-Fi: ");
  Serial.println(savedSSID);

  unsigned long startTime = millis();

 while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeoutMs) {
  ledStatusWiFiConnecting();
  Serial.print(".");
}

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    configMode = false;

    Serial.println("Wi-Fi connected");
    Serial.print("ESP32 IP address: ");
    Serial.println(WiFi.localIP());

    return true;
  }

  Serial.println("Wi-Fi failed. Starting setup AP...");
  startSetupAP();
  return false;
}

void handleSaveConfig(WebServer &server) {
  if (!requireLogin(server)) {
  return;
}

  String oldSSID = savedSSID;

  if (server.hasArg("ssid")) {
    savedSSID = server.arg("ssid");
  }

  if (server.hasArg("pass")) {
    String newPass = server.arg("pass");

    if (configMode || savedSSID != oldSSID || newPass.length() > 0) {
      savedPASS = newPass;
    }
  }

  if (server.hasArg("devname")) {
    savedDeviceName = server.arg("devname");

    if (savedDeviceName.length() == 0) {
      savedDeviceName = "esp32-led";
    }
  }

  if (server.hasArg("otapass")) {
    String newOtaPass = server.arg("otapass");

    if (newOtaPass.length() > 0) {
      savedOtaPassword = newOtaPass;
    }
  }

  if (server.hasArg("adminuser")) {
    savedAdminUser = server.arg("adminuser");

    if (savedAdminUser.length() == 0) {
      savedAdminUser = "admin";
    }
  }

  if (server.hasArg("adminpass")) {
    String newAdminPass = server.arg("adminpass");

    if (newAdminPass.length() > 0) {
      savedAdminPassword = newAdminPass;
    }
  }

  configPrefs.putString("ssid", savedSSID);
  configPrefs.putString("pass", savedPASS);
  configPrefs.putString("devname", savedDeviceName);
  configPrefs.putString("otapass", savedOtaPassword);
  configPrefs.putString("adminuser", savedAdminUser);
  configPrefs.putString("adminpass", savedAdminPassword);

  String mainUrl = "http://" + savedDeviceName + ".local";

  String response = "";
  response += "<!DOCTYPE html>";
  response += "<html>";
  response += "<head>";
  response += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  response += "<title>Restarting ESP32</title>";

  response += "<style>";
  response += "body{margin:0;font-family:Arial;background:#020617;color:white;text-align:center;}";
  response += ".box{max-width:430px;margin:60px auto;padding:25px;}";
  response += "h1{color:#22d3ee;}";
  response += "p{color:#cbd5e1;font-size:16px;line-height:1.5;}";
  response += "a{display:block;margin-top:20px;padding:15px;border-radius:14px;background:#0f766e;color:white;text-decoration:none;font-weight:bold;}";
  response += ".small{font-size:13px;color:#94a3b8;margin-top:18px;}";
  response += "</style>";

  response += "<script>";
  response += "let count = 12;";
  response += "function startTimer(){";
  response += "document.getElementById('timer').innerHTML = count;";
  response += "let t = setInterval(function(){";
  response += "count--;";
  response += "document.getElementById('timer').innerHTML = count;";
  response += "if(count <= 0){";
  response += "clearInterval(t);";
  response += "window.location.href = '" + mainUrl + "';";
  response += "}";
  response += "},1000);";
  response += "}";
  response += "</script>";

  response += "</head>";
  response += "<body onload='startTimer()'>";
  response += "<div class='box'>";
  response += "<h1>Configuration Saved</h1>";
  response += "<p>ESP32 is restarting and connecting to Wi-Fi.</p>";
  response += "<p>Main webpage will open automatically in <b><span id='timer'>12</span></b> seconds.</p>";
  response += "<a href='" + mainUrl + "'>Open Main Webpage</a>";
  response += "<div class='small'>";
  response += "If page does not open, connect phone/laptop to same Wi-Fi and open:<br><br>";
  response += "<b>" + mainUrl + "</b>";
  response += "</div>";
  response += "</div>";
  response += "</body>";
  response += "</html>";

  server.send(200, "text/html", response);

  delay(2000);
  ESP.restart();
}

void handleResetConfig(WebServer &server) {
if (!requireLogin(server)) {
  return;
}

  configPrefs.clear();

  String response = "";
  response += "<html><body style='font-family:Arial;text-align:center;background:#020617;color:white;'>";
  response += "<h2>Configuration Reset</h2>";
  response += "<p>ESP32 is restarting...</p>";
  response += "</body></html>";

  server.send(200, "text/html", response);

  delay(1500);
  ESP.restart();
}

void setupConfigRoutes(WebServer &server) {
  server.on("/settings", HTTP_GET, [&server]() {
 if (!requireLogin(server)) {
  return;
}

   server.send(200, "text/html; charset=utf-8", getConfigPageHtml());
  });

  server.on("/saveconfig", HTTP_POST, [&server]() {
    handleSaveConfig(server);
  });

  server.on("/resetconfig", HTTP_POST, [&server]() {
    handleResetConfig(server);
  });
}


void factoryResetConfig() {
  Serial.println("FACTORY RESET STARTED");

  configPrefs.clear();

  Preferences ledPrefs;
  ledPrefs.begin("led_state", false);
  ledPrefs.clear();
  ledPrefs.end();

  Serial.println("Wi-Fi SSID/password cleared");
  Serial.println("Admin username/password cleared");
  Serial.println("OTA password cleared");
  Serial.println("Device name cleared");
  Serial.println("LED state cleared");
  Serial.println("Restarting ESP32...");

  delay(1000);
  ESP.restart();
}