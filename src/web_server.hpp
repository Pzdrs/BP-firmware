#pragma once

#include <nlohmann/json.hpp>
#include "ESPAsyncWebServer.h"

using JSON = nlohmann::json;

extern AsyncWebServer server;

void setupWebServer();
