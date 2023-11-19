#include "mqtt_endpoint_handlers.hpp"
#include "web_server.hpp"
#include "Preferences.h"

Preferences preferences;

JSON getMqttConfig() {
    preferences.begin("mqtt", true);
    JSON config = JSON{
            {"server",   preferences.getString("server").c_str()},
            {"port",     preferences.getString("port").c_str()},
            {"username", preferences.getString("username").c_str()},
            {"password", preferences.getString("password").c_str()},
    };
    preferences.end();
    return config;
}

void updateProperty(const String &key, AsyncWebServerRequest *request) {
    if (request->hasParam(key, true)) {
        String value = request->getParam(key, true)->value();
        if (preferences.getString(key.c_str()) != value) preferences.putString(key.c_str(), value);
    }
}

void testMqtt(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", JSON{{"success", true}}.dump().c_str());
}

void clearMqttConfig(AsyncWebServerRequest *request) {
    preferences.begin("mqtt", false);
    preferences.clear();
    preferences.end();
    request->send(200, "application/json", JSON{{"success", true}}.dump().c_str());
}

void updateMqttConfig(AsyncWebServerRequest *request) {
    preferences.begin("mqtt", false);

    updateProperty("username", request);
    updateProperty("password", request);
    updateProperty("server", request);
    updateProperty("port", request);

    request->send(
            200,
            "application/json",
            JSON{
                    {"success", true},
                    {"config",  getMqttConfig()}
            }.dump().c_str()
    );
    preferences.end();
}

void mqttConfig(AsyncWebServerRequest *request) {
    request->send(
            200,
            "application/json",
            getMqttConfig().dump().c_str()
    );
}
