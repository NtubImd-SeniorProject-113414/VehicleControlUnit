#include "espnow_manager.h"

const uint8_t peerMACAddress[] = {0x08, 0xA6, 0xF7, 0x48, 0x69, 0x94}; // 目標 ESP32 的 MAC 地址

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // 顯示本機 MAC 地址
  String macAddress = WiFi.macAddress();
  Serial.print("ESP32 MAC Address: ");
  Serial.println(macAddress);

  // 初始化 ESP-NOW
  initESPNow();

  // 如果需要和其他設備通信，添加對等設備
  addPeer(peerMACAddress); 
}

void loop() {
  const char* message = "Hello from ESP32";
  
  if (sendCommand(peerMACAddress, (uint8_t*)message, strlen(message))) {
      Serial.println("Message sent successfully");
  } else {
      Serial.println("Failed to send message");
  }
  delay(2000);  // 每2秒發送一次消息
}
