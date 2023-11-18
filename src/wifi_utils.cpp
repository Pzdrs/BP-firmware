#include "wifi_utils.hpp"
#include <WiFi.h>

JSON getCurrentWifiNetwork() {
    if (!WiFi.isConnected()) return nullptr;
    return JSON{
            {"address_v4", WiFi.localIP().toString().c_str()}
    };
}

JSON getAvailableWifiNetworks() {
    JSON networks;
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
    return JSON::array();
}

bool getWifiState() {
    int wifiMode = WiFiClass::getMode();
    bool wifiSTAEnabled = wifiMode == WIFI_MODE_STA || wifiMode == WIFI_MODE_APSTA;
    return wifiSTAEnabled;
}