#include <LittleFS.h>
#include "ESPAsyncWebServer.h"
#include "endpoint_handlers/wifi_endpoint_handlers.hpp"
#include "endpoint_handlers/mqtt_endpoint_handlers.hpp"

AsyncWebServer server(80);

void home(AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
}

String api(const String &route) {
    return "/api" + route;
}

String wl(const String &route) {
    return api("/wifi" + route);
}

String mqtt(const String &route) {
    return api("/mqtt" + route);
}

void attachWebRoutes() {
    // CORS preflight workaround
    server.on("*", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        request->send(200);
    });

    // The main front end entry point
    server.on("/", HTTP_GET, home);

    // API endpoints
    server.on(wl("/refresh").c_str(), HTTP_POST, refreshNetworks);
    server.on(wl("/disconnect").c_str(), HTTP_POST, disconnectWifi);
    server.on(wl("/connect").c_str(), HTTP_POST, connectWifi);
    server.on(wl("/toggle-state").c_str(), HTTP_POST, toggleWifi);
    server.on(wl("").c_str(), HTTP_GET, wifiStatus);

    server.on(mqtt("/test").c_str(), HTTP_GET, testMqtt);
    server.on(mqtt("").c_str(), HTTP_GET, mqttConfig);
    server.on(mqtt("").c_str(), HTTP_POST, updateMqttConfig);


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

