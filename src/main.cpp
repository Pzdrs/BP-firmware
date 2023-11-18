#include <LittleFS.h>
#include <WiFi.h>
#include "web_server.hpp"
#include "TinyGPS++.h"

#define LOCAL_IP IPAddress(192, 168, 0, 1)
#define SLASH_24 IPAddress(255, 255, 255, 0)

TinyGPSPlus gps;

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

void setup() {
    Serial1.begin(9600);
    Serial.begin(115200);

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    WiFi.begin("IOT", "remeslovkostce_iot");

    while (WiFiClass::status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    WiFi.softAPConfig(LOCAL_IP, LOCAL_IP, SLASH_24);
    WiFi.softAP(("ESGPS-" + WiFi.macAddress()).c_str());

    Serial.println(WiFi.localIP());

    WiFi.scanNetworks(true);

    setupWebServer();
}

void loop() {
    while (Serial1.available() > 0) gps.encode(Serial1.read());
}


