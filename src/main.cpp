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

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");

            Serial.print(file.name());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900,
                          (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min,
                          tmstruct->tm_sec);

            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");

            Serial.print(file.size());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900,
                          (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min,
                          tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}

bool isIpAddress(const String &str) {
    for (int i = 0; i < str.length(); i++) {
        if (str.charAt(i) == '.') continue;
        if (!isDigit(str.charAt(i))) return false;
    }
    return true;
}

void setup() {
    Serial1.begin(9600);
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

    WiFi.begin("IOT", "remeslovkostce_iot");

    while (WiFiClass::status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    digitalWrite(WIFI_LED, HIGH);

    WiFi.softAPConfig(LOCAL_IP, LOCAL_IP, SLASH_24);

    Serial.println(WiFi.localIP());

    WiFi.scanNetworks(true);

    setupWebServer();

    preferences.begin("mqtt", true);
    String username = preferences.getString("username");
    String password = preferences.getString("password");
    String broker = preferences.getString("server");
    uint16_t port = 1883;// preferences.getString("port").toInt();

    Serial.println(username);
    Serial.println(password);
    Serial.println(broker);
    Serial.println(port);

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

    Serial.println(mqttClient.state());

    preferences.end();
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

    if (lastMillisMqttLed != 0 && currentMillis - lastMillisMqttLed >= MQTT_LED_BLINK_PERIOD) {
        digitalWrite(PUBLISH_LED, LOW);
        lastMillisMqttLed = 0;
    }


    while (Serial1.available() > 0) gps.encode(Serial1.read());

    if (currentMillis - lastMillisWs >= GNSS_DATA_SUBMIT_PERIOD) {
        lastMillisWs = currentMillis;
        Serial.println("Sending data to WS");
        JSON location = {
                {"type", "location"},
                {"data", JSON{
                        {"lat",    gps.location.lat()},
                        {"lng",    gps.location.lng()},
                        {"alt",    gps.altitude.meters()},
                        {"speed",  gps.speed.kmph()},
                        {"course", gps.course.deg()}
                }}
        };
        Serial.println(location.dump().c_str());
        gnssWs.textAll(location.dump().c_str());
    }

    if (currentMillis - lastMillisMqtt >= GNSS_DATA_SUBMIT_PERIOD) {
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



