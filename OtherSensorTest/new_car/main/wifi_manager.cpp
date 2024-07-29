#include <WiFi.h>
#include <WebServer.h>
#include "wifi_manager.h"

// AP 模式配置
const char* ap_ssid = "ESP32-Setup";
const char* ap_password = "12345678";

WebServer server(80);
bool wifi_connected = false;

void startWiFi() {
  String ssid = ""; // 從EEPROM中讀取
  String password = ""; // 從EEPROM中讀取

  if (ssid.length() > 0) {
    WiFi.begin(ssid.c_str(), password.c_str());
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      Serial.println("Connected to Wi-Fi");
      wifi_connected = true;
    } else {
      Serial.println("Failed to connect to Wi-Fi");
      startAPMode();
    }
  } else {
    startAPMode();
  }
}

void startAPMode() {
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("AP Mode Started. Connect to AP and visit 192.168.4.1");

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<form action=\"/save\" method=\"post\">SSID:<input name=\"ssid\" length=32><br>Password:<input name=\"password\" length=64><br><input type=\"submit\"></form>");
  });

  server.on("/save", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    // 保存至EEPROM
    server.send(200, "text/html", "<h1>Saved! Rebooting...</h1>");
    delay(2000);
    ESP.restart();
  });

  server.begin();
}

void handleWiFi() {
  if (!wifi_connected) {
    server.handleClient();
  }
}
