#include <LittleFS.h>
#include <WiFi.h>
#include "web_server.hpp"
#include "TinyGPS++.h"
#include "PubSubClient.h"
#include "ezButton.h"

#define LOCAL_IP IPAddress(192, 168, 0, 1)
#define SLASH_24 IPAddress(255, 255, 255, 0)

#define GNSS_DATA_SUBMIT_PERIOD 1000
#define MQTT_LED_BLINK_PERIOD 100

#define HOTSPOT_LED 5
#define WIFI_LED 2
#define GNSS_FIX_LED 4
#define PUBLISH_LED 18

const int ledPins[] = {HOTSPOT_LED, WIFI_LED, GNSS_FIX_LED, PUBLISH_LED};

ezButton hotspotButton(15);

TinyGPSPlus gps;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

bool isIpAddress(const String &str) {
    for (int i = 0; i < str.length(); i++) {
        if (str.charAt(i) == '.') continue;
        if (!isDigit(str.charAt(i))) return false;
    }
    return true;
}

bool attemptWifiAutoConnect() {
    preferences.begin("wifi", false);
    auto ssid = preferences.isKey("ssid") ? preferences.getString("ssid") : "";
    auto password = preferences.isKey("password") ? preferences.getString("password") : "";
    preferences.end();

    if (ssid.length() > 0) {
        Serial.println("Found saved WiFi credentials");
        WiFi.begin(ssid.c_str(), password.c_str());

        int tries = 0;
        do {
            Serial.println("Connecting to WiFi..");
            tries++;
            delay(1000);
        } while (WiFiClass::status() != WL_CONNECTED && tries < 15);

        if (WiFiClass::status() == WL_CONNECTED) {
            Serial.println("Connected to " + WiFi.SSID() + " (" + WiFi.localIP().toString() + ")");
            digitalWrite(WIFI_LED, HIGH);
            return true;
        } else {
            Serial.println("Failed to connect to " + ssid);
        }
        return false;
    }
    Serial.println("No saved WiFi credentials found");
    return false;
}

void setupMqtt() {
    preferences.begin("mqtt", true);
    String username = preferences.getString("username");
    String password = preferences.getString("password");
    String broker = preferences.getString("server");
    uint16_t port = preferences.getString("port").toInt();

    if (isIpAddress(broker)) {
        IPAddress ip;
        if (ip.fromString(broker)) {
            mqttClient.setServer(ip, port);
        }
    } else {
        mqttClient.setServer(broker.c_str(), port);
    }

    if (mqttClient.connect("ESP", username.c_str(), password.c_str())) {
        Serial.println("Connected to MQTT");
    } else {
        Serial.println("Failed to connect to MQTT");
    }

    preferences.end();
}

void setup() {
    Serial1.begin(9600, SERIAL_8N1, 16, 17);
    Serial.begin(115200);

    // LED init
    for (const auto &item: ledPins) {
        pinMode(item, OUTPUT);
        digitalWrite(item, LOW);
    }

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    // WiFi
    WiFi.scanNetworks(true);
    WiFi.softAPConfig(LOCAL_IP, LOCAL_IP, SLASH_24);
    bool wifiConnected = attemptWifiAutoConnect();

    if (wifiConnected) setupMqtt();
    setupWebServer();
}


void loop() {
    static uint32_t lastMillisMqtt, lastMillisWs, lastMillisMqttLed;
    static bool hotspotButtonWasPressed, hotspotState;
    unsigned long currentMillis = millis();

    hotspotButton.loop();

    if (hotspotButton.isPressed()) {
        hotspotButtonWasPressed = true;
    }

    if (hotspotButton.isReleased() && hotspotButtonWasPressed) {
        Serial.println("Toggling hotspot");
        hotspotState = !hotspotState;
        if (hotspotState) {
            WiFi.softAP(("ESGPS-" + WiFi.macAddress()).c_str());
            digitalWrite(HOTSPOT_LED, HIGH);
            Serial.println("WiFi hotspot enabled");
        } else {
            WiFi.softAPdisconnect(true);
            digitalWrite(HOTSPOT_LED, LOW);
            Serial.println("WiFi hotspot disabled");
        }
        hotspotButtonWasPressed = false;
        delay(10);
    }

    if (
            WiFiClass::status() == WL_CONNECTED &&
            (lastMillisMqttLed != 0 && currentMillis - lastMillisMqttLed >= MQTT_LED_BLINK_PERIOD)
            ) {
        digitalWrite(PUBLISH_LED, LOW);
        lastMillisMqttLed = 0;
    }


    if (Serial1.available()) {
        auto data = Serial1.read();
        gps.encode(data);
    }

    if(gps.satellites.value() >= 4) {
        digitalWrite(GNSS_FIX_LED, HIGH);
    } else {
        digitalWrite(GNSS_FIX_LED, LOW);
    }

    if (!gps.location.isUpdated()) return;

    if (currentMillis - lastMillisWs >= GNSS_DATA_SUBMIT_PERIOD) {
        lastMillisWs = currentMillis;
        JSON location = {
                {"type", "location"},
                {"data", JSON{
                        {"lat",        gps.location.lat()},
                        {"lng",        gps.location.lng()},
                        {"alt",        gps.altitude.meters()},
                        {"speed",      gps.speed.kmph()},
                        {"course",     gps.course.deg()},
                        {"satellites", gps.satellites.value()},
                        {"valid",      gps.location.isValid() ? "true" : "false"}

                }}
        };
        Serial.println(location.dump().c_str());
        gnssWs.textAll(location.dump().c_str());
    }

    if (WiFiClass::status() == WL_CONNECTED && mqttClient.connected() && (currentMillis - lastMillisMqtt >= GNSS_DATA_SUBMIT_PERIOD)) {
        lastMillisMqtt = currentMillis;
        lastMillisMqttLed = currentMillis;
        digitalWrite(PUBLISH_LED, HIGH);
        mqttClient.publish("gnss", JSON{
                {"source", WiFi.macAddress().c_str()},
                {"lat",    gps.location.lat()},
                {"lng",    gps.location.lng()},
                {"alt",    gps.altitude.meters()},
                {"speed",  gps.speed.kmph()},
                {"course", gps.course.deg()}
        }.dump().c_str());
    }
    mqttClient.loop();
}



