#include <nlohmann/json.hpp>
#include <WiFi.h>
#include "wifi_endpoint_handlers.hpp"

#define SSID_PARAM "ssid"
#define PASSWORD_PARAM "password"

using json = nlohmann::json;

json getCurrentWifiNetwork() {
    if (!WiFi.isConnected()) return nullptr;
    return json(
            {
                    {"address_v4", WiFi.localIP().toString().c_str()}
            }
    );
}

json getAvailableWifiNetworks() {
    json networks;
    int n = WiFi.scanComplete();
    if (n) {
        for (int i = 0; i < n; ++i) {
            networks.push_back(
                    {
                            {"rssi",       WiFi.RSSI(i)},
                            {"ssid",       WiFi.SSID(i).c_str()},
                            {"bssid",      WiFi.BSSIDstr(i).c_str()},
                            {"channel",    WiFi.channel(i)},
                            {"encryption", WiFi.encryptionType(i)},
                            {"connected",  WiFi.BSSIDstr() == WiFi.BSSIDstr(i)}
                    }
            );
        }
        return networks;
    }
    return json::array();
}


void wifiStatus(AsyncWebServerRequest *request) {
    json response = {
            {"enabled",  true},
            {"network",  getCurrentWifiNetwork()},
            {"networks", getAvailableWifiNetworks()}
    };
    request->send(200, "application/json", response.dump().c_str());
}


void refreshNetworks(AsyncWebServerRequest *request) {
    WiFi.scanDelete();
    WiFi.scanNetworks(true);
    request->send(200);
}

void toggleWifi(AsyncWebServerRequest *request) {
    int wifiMode = WiFiClass::getMode();
    bool wifiSTAEnabled = wifiMode == WIFI_MODE_STA || wifiMode == WIFI_MODE_APSTA;

    if (WiFi.enableSTA(!wifiSTAEnabled)) {
        request->send(
                200,
                "application/json",
                json({
                             {"success",  true},
                             {"newState", !wifiSTAEnabled}}
                ).dump().c_str()
        );
    } else {
        request->send(
                400,
                "application/json",
                json({{"success", false}}).dump().c_str()
        );
    }

}

void disconnectWifi(AsyncWebServerRequest *request) {
    bool result = WiFi.disconnect();
    request->send(
            200,
            "application/json",
            json({{"success", result}}).dump().c_str()
    );
}

void connectWifi(AsyncWebServerRequest *request) {
    if (!request->hasParam(SSID_PARAM, true) || !request->hasParam(PASSWORD_PARAM, true)) {
        request->send(
                400,
                "application/json",
                json({
                             {"success", false},
                             {"message", "Missing ssid or password parameter"}
                     }
                ).dump().c_str()
        );
        return;
    }

    String ssid = request->getParam(SSID_PARAM, true)->value();
    String password = request->getParam(PASSWORD_PARAM, true)->value();
    uint8_t result = WiFi.begin(ssid.c_str(), password.c_str());
    if (result == WL_CONNECTED) {
        request->send(
                200,
                "application/json",
                json({
                             {"success", true},
                             {"address", WiFi.localIP().toString()}
                     }
                ).dump().c_str()
        );
    } else {
        request->send(
                400,
                "application/json",
                json({
                             {"success", false},
                             {"message", "Failed to connect to wifi"}
                     }
                ).dump().c_str()
        );
    }
}