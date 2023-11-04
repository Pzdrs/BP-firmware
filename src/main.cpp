#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <esp_err.h>
#include <mesh_util.h>
#include <esp_http_server.h>
#include <WebServer.h>

//AsyncWebServer server(80);



///* Our URI handler function to be called during GET /uri request */
//esp_err_t get_handler(httpd_req_t *req) {
//    /* Send a simple response */
//    const char resp[] = "URI GET Response";
//    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
//    return ESP_OK;
//}
//
///* Our URI handler function to be called during POST /uri request */
//esp_err_t post_handler(httpd_req_t *req) {
//    /* Destination buffer for content of HTTP POST request.
//     * httpd_req_recv() accepts char* only, but content could
//     * as well be any binary data (needs type casting).
//     * In case of string data, null termination will be absent, and
//     * content length would give length of string */
//    char content[100];
//
//    /* Truncate if content length larger than the buffer */
//    size_t recv_size = MIN(req->content_len, sizeof(content));
//
//    int ret = httpd_req_recv(req, content, recv_size);
//    if (ret <= 0) {  /* 0 return value indicates connection closed */
//        /* Check if timeout occurred */
//        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//            /* In case of timeout one can choose to retry calling
//             * httpd_req_recv(), but to keep it simple, here we
//             * respond with an HTTP 408 (Request Timeout) error */
//            httpd_resp_send_408(req);
//        }
//        /* In case of error, returning ESP_FAIL will
//         * ensure that the underlying socket is closed */
//        return ESP_FAIL;
//    }
//
//    /* Send a simple response */
//    const char resp[] = "URI POST Response";
//    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
//    return ESP_OK;
//}
//
///* URI handler structure for GET /uri */
//httpd_uri_t uri_get = {
//        .uri      = "/uri",
//        .method   = static_cast<httpd_method_t>(0b00000001),
//        .handler  = get_handler,
//        .user_ctx = nullptr
//};
//
///* URI handler structure for POST /uri */
//httpd_uri_t uri_post = {
//        .uri      = "/uri",
//        .method   = static_cast<httpd_method_t>(0b00000010),
//        .handler  = post_handler,
//        .user_ctx = nullptr
//};
//
///* Function for starting the webserver */
//httpd_handle_t start_webserver() {
//    /* Generate default configuration */
//    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
//
//    /* Empty handle to esp_http_server */
//    httpd_handle_t server = nullptr;
//
//    /* Start the httpd server */
//    if (httpd_start(&server, &config) == ESP_OK) {
//        /* Register URI handlers */
//        httpd_register_uri_handler(server, &uri_get);
//        httpd_register_uri_handler(server, &uri_post);
//    }
//    /* If server failed to start, handle will be NULL */
//    return server;
//}
//
///* Function for stopping the webserver */
//void stop_webserver(httpd_handle_t server) {
//    if (server) {
//        /* Stop the httpd server */
//        httpd_stop(server);
//    }
//}

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
    Serial.begin(115200);

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    listDir(LittleFS, "/", 0);

    WiFi.begin("IOT", "remeslovkostce_iot");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());

    //start_webserver();

//    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//        request->send(LittleFS, "/index.html", "text/html");
//    });
//
//    server.begin();
}

void loop() {

}


