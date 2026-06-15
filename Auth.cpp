#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "LED.h"
#include "Auth.h"
#include "Config.h"
#include "LoginPage.h"

String sessionToken = "";

String makeSessionToken() {
  String token = "";
  token += String((uint32_t)ESP.getEfuseMac(), HEX);
  token += String(millis(), HEX);
  token += String(random(100000, 999999), HEX);
  return token;
}

String getCookieValue(String cookie, String name) {
  int start = cookie.indexOf(name + "=");

  if (start < 0) {
    return "";
  }

  start += name.length() + 1;

  int end = cookie.indexOf(";", start);

  if (end < 0) {
    end = cookie.length();
  }

  return cookie.substring(start, end);
}

bool isLoggedIn(WebServer &server) {
  if (isConfigMode()) {
    return true;
  }

  if (sessionToken.length() == 0) {
    return false;
  }

  if (!server.hasHeader("Cookie")) {
    return false;
  }

  String cookie = server.header("Cookie");
  String token = getCookieValue(cookie, "ESPSESSIONID");

  if (token == sessionToken) {
    return true;
  }

  return false;
}

bool requireLogin(WebServer &server) {
  if (isLoggedIn(server)) {
    return true;
  }

  server.sendHeader("Location", "/login");
  server.send(302, "text/plain", "Redirecting to login...");
  return false;
}

void handleLoginPage(WebServer &server) {
  if (isLoggedIn(server)) {
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "Already logged in");
    return;
  }

  server.send_P(200, "text/html; charset=utf-8", LOGIN_PAGE);
}

void handleLoginCheck(WebServer &server) {
  if (!server.hasArg("username") || !server.hasArg("password")) {
    server.sendHeader("Location", "/login?error=1");
    server.send(302, "text/plain", "Missing login data");
    return;
  }

  String username = server.arg("username");
  String password = server.arg("password");

  if (username == getAdminUser() && password == getAdminPassword()) {
    ledStatusLoginSuccess();
    sessionToken = makeSessionToken();

    String cookie = "ESPSESSIONID=" + sessionToken + "; Path=/; HttpOnly; SameSite=Lax";

    server.sendHeader("Set-Cookie", cookie);
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "Login success");
    return;
  }

  server.sendHeader("Location", "/login?error=1");
  server.send(302, "text/plain", "Wrong username or password");
}

void handleLogout(WebServer &server) {
  sessionToken = "";

  server.sendHeader("Set-Cookie", "ESPSESSIONID=deleted; Path=/; Max-Age=0");
  server.sendHeader("Location", "/login");
  server.send(302, "text/plain", "Logged out");
}

void setupAuthRoutes(WebServer &server) {
  const char *headerKeys[] = {"Cookie"};
  server.collectHeaders(headerKeys, 1);

  randomSeed(analogRead(0));

  server.on("/login", HTTP_GET, [&server]() {
    handleLoginPage(server);
  });

  server.on("/logincheck", HTTP_POST, [&server]() {
    handleLoginCheck(server);
  });

  server.on("/logout", HTTP_GET, [&server]() {
    handleLogout(server);
  });
}