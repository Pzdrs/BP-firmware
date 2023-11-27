#include "web_server.hpp"
#include <LittleFS.h>
#include "ESPAsyncWebServer.h"
#include "endpoint_handlers/wifi_endpoint_handlers.hpp"
#include "endpoint_handlers/mqtt_endpoint_handlers.hpp"
#include "wifi_utils.hpp"

AsyncWebServer server(80);
AsyncWebSocket gnssWs("/gnss");
Preferences preferences;

void status(AsyncWebServerRequest *request) {
    request->send(200, "application/json", JSON(
            {
                    {"wifi",     JSON{
                            {"connected", true},
                            {"rssi",      -25}
                    }},
                    {"cellular", JSON{
                            {"connected", true},
                            {"rssi",      -25}
                    }},
                    {"gnss",     JSON{
                            {"satellites", 10}
                    }}
            }
    ).dump().c_str());
}


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

void onEvent(AsyncWebSocket *webSocket,
             AsyncWebSocketClient *client,
             AwsEventType type,
             void *arg,
             uint8_t *data,
             size_t len) {

    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                          client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void setupWebSockets() {
    gnssWs.onEvent(onEvent);
    server.addHandler(&gnssWs);
}

void attachWebRoutes() {
    // The main front end entry point
    server.on("/", HTTP_GET, home);
    // Static content
    server.serveStatic("/assets", LittleFS, "/assets");


    server.on(api("/status").c_str(), HTTP_GET, status);
    server.on(api("/power/restart").c_str(), HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", JSON{{"success", true}}.dump().c_str());
        ESP.restart();
    });

    server.on(wl("/disconnect").c_str(), HTTP_POST, disconnectWifi);
    server.on(wl("/connect").c_str(), HTTP_POST, connectWifi);
    server.on(wl("/toggle-state").c_str(), HTTP_POST, toggleWifi);
    server.on(wl("").c_str(), HTTP_GET, wifiStatus);

    server.on(mqtt("/test").c_str(), HTTP_GET, testMqtt);
    server.on(mqtt("").c_str(), HTTP_GET, mqttConfig);
    server.on(mqtt("").c_str(), HTTP_POST, updateMqttConfig);
    server.on(mqtt("").c_str(), HTTP_DELETE, clearMqttConfig);

}

void attachHandlers() {
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Nemame pane");
    });
}

void setupCors() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

    // CORS preflight workaround
    server.on("*", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        request->send(200);
    });
}

void setupWebServer() {
    setupCors();
    setupWebSockets();
    attachWebRoutes();
    attachHandlers();
    server.begin();
}

