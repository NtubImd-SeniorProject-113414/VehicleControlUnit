#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "wifi_manager.h"

const char* ap_ssid = "MediMateIot-Setup";
const char* ap_password = "adminaccess";

WebServer server(80);
Preferences preferences;
bool wifi_connected = false;

void startWiFi() {
  //clearAllWiFiSettings();
  preferences.begin("wifiCreds", true);  // 打開只讀模式
  String ssid = preferences.getString("ssid_0", ""); // 從 Preferences 中讀取SSID
  String password = preferences.getString("password_0", ""); // 從 Preferences 中讀取密碼
  preferences.end();

  if (ssid.length() > 0) {
      WiFi.begin(ssid.c_str(), password.c_str());
      if (WiFi.waitForConnectResult() == WL_CONNECTED) {
          Serial.println("Connected to Wi-Fi");
          wifi_connected = true;
      } else {
          Serial.println("Failed to connect to Wi-Fi");
          startAPMode();  // 啟動 AP 模式
      }
  } else {
      startAPMode();  // 啟動 AP 模式
  }
}

void startAPMode() {
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    Serial.println("AP Mode Started. Connect to AP and visit 192.168.4.1");

    server.on("/", HTTP_GET, []() {
        String page = "<html><head><title>MediMate設備連線設定</title><meta charset=\"UTF-8\"><style>"
                      "body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }"
                      "h1 { font-size: 24px; color: #333; }"
                      "form { margin: 20px auto; display: inline-block; text-align: left; }"
                      "input[type='text'], input[type='password'] {"
                      "width: 100%; padding: 10px; margin: 10px 0; box-sizing: border-box; font-size: 16px; }"
                      "input[type='submit'] { width: 100%; background-color: #4CAF50; color: white; padding: 10px; font-size: 16px; border: none; cursor: pointer; }"
                      "input[type='submit']:hover { background-color: #45a049; }"
                      ".wifi-list { margin-top: 20px; text-align: left; display: inline-block; }"
                      ".wifi-list h2 { font-size: 18px; color: #555; }"
                      ".wifi-list ul { list-style-type: none; padding: 0; }"
                      ".wifi-list li { padding: 8px 0; font-size: 16px; }"
                      "button { background-color: #f44336; color: white; padding: 5px 10px; border: none; cursor: pointer; }"
                      "button:hover { background-color: #e53935; }"
                      "</style></head><body><h1>MediMate設備連線設定</h1><br>"
                      "<form action=\"/save\" method=\"post\">"
                      "SSID:<br><input type=\"text\" name=\"ssid\" length=32><br>"
                      "Password:<br><input type=\"password\" name=\"password\" length=64><br>"
                      "<input type=\"submit\" value=\"儲存\"></form>"
                      "<br><div class=\"wifi-list\"><h2>已新增的 WiFi 帳密：</h2><ul>";

        // 從 Preferences 中讀取已儲存的 WiFi 資訊
        preferences.begin("wifiCreds", true);
        int count = preferences.getInt("count", 0);
        for (int i = 0; i < count; i++) {
            String ssid = preferences.getString(("ssid_" + String(i)).c_str(), "");
            if (ssid.length() > 0) {
                page += "<li>" + ssid + " <button onclick=\"location.href='/delete?index=" + String(i) + "'\">刪除</button></li>";
            }
        }
        preferences.end();

        page += "</ul></div></body></html>";

        server.send(200, "text/html", page);
    });

    server.on("/save", HTTP_POST, []() {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        Serial.println("Received SSID: " + ssid);
        Serial.println("Received Password: " + password);

        // 保存到 Preferences 中
        preferences.begin("wifiCreds", false);  // 打開可讀寫模式
        int count = preferences.getInt("count", 0);
        preferences.putString(("ssid_" + String(count)).c_str(), ssid);
        preferences.putString(("password_" + String(count)).c_str(), password);

        preferences.putInt("count", count + 1);
        preferences.end();

        server.send(200, "text/html", "<meta charset=\"UTF-8\"><h1 style='text-align: left;'>已儲存</h1>");
        delay(2000);
        ESP.restart();
    });

    server.on("/delete", HTTP_GET, []() {
        String indexStr = server.arg("index");
        int index = indexStr.toInt();

        // 在 Preferences 中刪除特定的 WiFi 資訊
        preferences.begin("wifiCreds", false);  // 打開可讀寫模式
        int count = preferences.getInt("count", 0);
        
        for (int i = index; i < count - 1; i++) {
            String next_ssid = preferences.getString(("ssid_" + String(i + 1)).c_str(), "");
            String next_password = preferences.getString(("password_" + String(i + 1)).c_str(), "");
            preferences.putString(("ssid_" + String(i)).c_str(), next_ssid);
            preferences.putString(("password_" + String(i)).c_str(), next_password);
        }
        preferences.remove(("ssid_" + String(count - 1)).c_str());
        preferences.remove(("password_" + String(count - 1)).c_str());
        preferences.putInt("count", count - 1);
        preferences.end();

        server.sendHeader("Location", "/");  // 重定向回主頁面
        server.send(303);
    });

    server.begin();
    Serial.println("Web server started");
}

bool handleWiFi() {
    if (wifi_connected && WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi disconnected");
        wifi_connected = false;
        startWiFi();
    }

    if (!wifi_connected) {
        if (WiFi.status() == WL_CONNECTED) {
            wifi_connected = true;
            Serial.println("Wi-Fi connected");
            return true;
        } else {
            return false;
        }
    }
    return true;
}

void clearAllWiFiSettings() {
    preferences.begin("wifiCreds", false);  // 打開可讀寫模式

    int count = preferences.getInt("count", 0);
    for (int i = 0; i < count; i++) {
        preferences.remove(("ssid_" + String(i)).c_str());
        preferences.remove(("password_" + String(i)).c_str());
    }
    preferences.putInt("count", 0);  // 重置 Wi-Fi 設定數量
    preferences.end();

    Serial.println("All Wi-Fi settings cleared.");
}


