#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "control.h"

void setup() {
  Serial.begin(115200);

  // 初始化控制模塊
  initControl();
  // 啟動Wi-Fi和MQTT
  startWiFi();
}

void loop() {
  handleWiFi();  // 處理Wi-Fi狀態和AP模式下的Web服務器
  //handleMQTT();  // 處理MQTT消息和重連
}

