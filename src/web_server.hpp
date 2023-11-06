#pragma once

#include "ESPAsyncWebServer.h"

extern AsyncWebServer server;

void home(AsyncWebServerRequest *request);

void api(AsyncWebServerRequest *request);

void get_config(AsyncWebServerRequest *request);

void update_config(AsyncWebServerRequest *request);

void setupWebServer();