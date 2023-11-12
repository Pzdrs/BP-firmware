#include <nlohmann/json.hpp>
#include "mqtt_endpoint_handlers.hpp"

using json = nlohmann::json;

void testMqtt(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", json({{"success", true}}).dump().c_str());
}

void updateMqttConfig(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", json({{"success", true}}).dump().c_str());
}

void mqttConfig(AsyncWebServerRequest *request) {
    request->send(
            200,
            "application/json",
            json({
                         {"server", "server"},
                         {"port", "port"},
                         {"username", "user"},
                         {"password", "pass"},
                 }).dump().c_str()
    );
}
