#include "wifi_endpoint_handlers.hpp"
#include <WiFi.h>
#include "wifi_utils.hpp"

void wifiStatus(AsyncWebServerRequest *request) {
    JSON response = {
            {"enabled",  getWifiState()},
            {"network",  getCurrentWifiNetwork()},
            {"networks", getAvailableWifiNetworks()}
    };
    request->send(200, "application/json", response.dump().c_str());
}

void toggleWifi(AsyncWebServerRequest *request) {
    bool wifiSTAEnabled = getWifiState();

    if (WiFi.enableSTA(!wifiSTAEnabled)) {
        request->send(
                200,
                "application/json",
                JSON({
                             {"success",  true},
                             {"newState", !wifiSTAEnabled}}
                ).dump().c_str()
        );
    } else {
        request->send(
                400,
                "application/json",
                JSON({{"success", false}}).dump().c_str()
        );
    }

}

void disconnectWifi(AsyncWebServerRequest *request) {
    bool result = WiFi.disconnect();
    digitalWrite(2, LOW); // turn off the LED
    request->send(
            200,
            "application/json",
            JSON({{"success", result}}).dump().c_str()
    );
}

void connectWifi(AsyncWebServerRequest *request) {
    if (!request->hasParam(SSID_PARAM, true) || !request->hasParam(PASSWORD_PARAM, true)) {
        request->send(
                400,
                "application/json",
                JSON({
                             {"success", false},
                             {"message", "Missing ssid or password parameter"}
                     }
                ).dump().c_str()
        );
        return;
    }

    String ssid = request->getParam(SSID_PARAM, true)->value();
    String password = request->getParam(PASSWORD_PARAM, true)->value();

    Serial.println("attempting to connect to wifi with ssid: " + ssid + " and password: " + password);
    WiFi.begin(ssid.c_str(), password.c_str());
    int attempts = 0;
    while (WiFiClass::status() != WL_CONNECTED && attempts < 3) {
        Serial.println("attempting to connect to wifi");
        delay(1000);
        attempts++;
    }

    if (WiFiClass::status() == WL_CONNECTED) {
        digitalWrite(2, HIGH); // turn on the LED
        request->send(
                200,
                "application/json",
                JSON({
                             {"success", true},
                             {"address", WiFi.localIP().toString()}
                     }
                ).dump().c_str()
        );
    } else {
        request->send(
                400,
                "application/json",
                JSON({
                             {"success", false},
                             {"message", "Failed to connect to wifi"}
                     }
                ).dump().c_str()
        );
    }
}