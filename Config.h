#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WebServer.h>

void configBegin();

bool connectWiFiFromConfig(unsigned long timeoutMs);

void setupConfigRoutes(WebServer &server);

String getDeviceName();
String getOtaPassword();

String getAdminUser();
String getAdminPassword();

bool isConfigMode();

String getConfigPageHtml();

bool requireAuthentication(WebServer &server);

void factoryResetConfig();

#endif