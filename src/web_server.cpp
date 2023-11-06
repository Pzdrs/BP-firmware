#include <LittleFS.h>
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);



void home(AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
}

void api(AsyncWebServerRequest *request) {
    /**
     * API JSON endpoint that displays health information
     */
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    response->print(R"({"status": "OK"})");
    request->send(response);
}

void get_config(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    response->print(R"({"status": "OK"})");
    request->send(response);
}


void update_config(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    response->print(R"({"status": "OK"})");
    request->send(response);
}

void attachWebRoutes() {
    // The main front end entry point
    server.on("/", HTTP_GET, home);

    // API endpoints
    server.on("/api", HTTP_GET, api);
    server.on("/api/config", HTTP_GET, get_config);
    server.on("/api/config", HTTP_POST, update_config);

    // Static content
    server.serveStatic("/assets", LittleFS, "/assets");

    // Handlers
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Nemame pane");
    });
}

void setupWebServer() {
    attachWebRoutes();
    server.begin();
}

