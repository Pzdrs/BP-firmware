#include <LittleFS.h>
#include "ESPAsyncWebServer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

void scan_wifi(AsyncWebServerRequest *request) {
       json networks;
    int n = WiFi.scanComplete();
    if (n) {
        for (int i = 0; i < n; ++i) {
            networks.push_back({
                {"rssi", WiFi.RSSI(i)},
                {"ssid", WiFi.SSID(i).c_str()},
                {"bssid", WiFi.BSSIDstr(i).c_str()},
                {"channel", WiFi.channel(i)},
                {"secure", WiFi.encryptionType(i)}
            });
        }
    }
    request->send(200, "application/json", networks.dump().c_str());
}

void attachWebRoutes() {
    // The main front end entry point
    server.on("/", HTTP_GET, home);

    // API endpoints
    server.on("/api/wifi/scan", HTTP_GET, scan_wifi);

    // Static content
    server.serveStatic("/assets", LittleFS, "/assets");

    // Handlers
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Nemame pane");
    });
}

void setupCors() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
}

void setupWebServer() {
    setupCors();
    attachWebRoutes();
    server.begin();
}

