#include <nlohmann/json.hpp>
#include "mqtt_endpoint_handlers.hpp"
#include "web_server.hpp"

void testMqtt(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", JSON{{"success", true}}.dump().c_str());
}

void updateMqttConfig(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", JSON{{"success", true}}.dump().c_str());
}

void mqttConfig(AsyncWebServerRequest *request) {
    request->send(
            200,
            "application/json",
            JSON{
                    {"server",   "server"},
                    {"port",     "port"},
                    {"username", "user"},
                    {"password", "pass"},
            }.dump().c_str()
    );
}
