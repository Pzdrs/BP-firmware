#include "ESPAsyncWebServer.h"

#define SSID_PARAM "ssid"
#define PASSWORD_PARAM "password"

void wifiStatus(AsyncWebServerRequest *request);

void toggleWifi(AsyncWebServerRequest *request);

void disconnectWifi(AsyncWebServerRequest *request);

void connectWifi(AsyncWebServerRequest *request);
