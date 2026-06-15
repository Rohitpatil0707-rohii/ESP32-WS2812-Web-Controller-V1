#ifndef AUTH_H
#define AUTH_H

#include <Arduino.h>
#include <WebServer.h>

void setupAuthRoutes(WebServer &server);

bool isLoggedIn(WebServer &server);
bool requireLogin(WebServer &server);

void handleLoginPage(WebServer &server);
void handleLoginCheck(WebServer &server);
void handleLogout(WebServer &server);

#endif