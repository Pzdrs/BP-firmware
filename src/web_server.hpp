#pragma once

#include <nlohmann/json.hpp>
#include "ESPAsyncWebServer.h"
#include "Preferences.h"

extern Preferences preferences;

using JSON = nlohmann::json;

extern AsyncWebServer server;
extern AsyncWebSocket gnssWs;

void setupWebServer();
