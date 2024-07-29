#include <WiFi.h>
#include <WebServer.h>
#include "wifi_manager.h"

const char* ap_ssid = "MediMateIot-Setup";
const char* ap_password = "adminaccess";

WebServer server(80);
bool wifi_connected = false;

void startWiFi() {
    String ssid = ""; // 從 EEPROM 中讀取
    String password = ""; // 從 EEPROM 中讀取

    if (ssid.length() > 0) {
        WiFi.begin(ssid.c_str(), password.c_str());
        if (WiFi.waitForConnectResult() == WL_CONNECTED) {
            Serial.println("Connected to Wi-Fi");
            wifi_connected = true;
        } else {
            Serial.println("Failed to connect to Wi-Fi");
            startAPMode();  // 正確調用 startAPMode
        }
    } else {
        startAPMode();  // 正確調用 startAPMode
    }
}

void startAPMode() {
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    Serial.println("AP Mode Started. Connect to AP and visit 192.168.4.1");
    
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", "<form action=\"/save\" method=\"post\">SSID:<input name=\"ssid\" length=32><br>Password:<input name=\"password\" length=64><br><input type=\"submit\"></form>");
    });

    server.on("/save", HTTP_POST, []() {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        Serial.println("Received SSID: " + ssid);
        Serial.println("Received Password: " + password);
        // 保存到 EEPROM 或其他存儲中
        server.send(200, "text/html", "<h1>Saved! Rebooting...</h1>");
        delay(2000);
        ESP.restart();
    });

    server.begin();
    Serial.println("Web server started");
}


void handleWiFi() {
    if (!wifi_connected) {
        server.handleClient();
    }
}
