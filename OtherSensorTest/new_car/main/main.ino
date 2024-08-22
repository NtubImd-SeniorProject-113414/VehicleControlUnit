#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "ESP32Comm.h"
#include "control.h"
#include <HardwareSerial.h>  // 包含 HardwareSerial 庫

HardwareSerial MySerial(1);  // 創建 HardwareSerial 物件，使用 UART1

const int rxPin = 35;      // RX pin (從其他 ESP32 接收數據)
const int txPin = 12;      // TX pin (傳送數據到其他 ESP32)
const int ledPin = 2;      // LED pin

ESP32Comm myComm(Serial1, rxPin, txPin, 9600);  // 創建通信物件，使用 UART1

void setup() {
  Serial.begin(115200);    // ESP32 與電腦的串口通信
  MySerial.begin(9600, SERIAL_8N1, rxPin, txPin);  // 初始化 HardwareSerial UART1，指定 RX 和 TX 引腳

  initControl();           // 初始化控制相關邏輯
  
  pinMode(ledPin, OUTPUT); // 初始化 LED pin
  digitalWrite(ledPin, LOW);
  
  startWiFi();   // 啟動 WiFi
  startMQTT();   // 啟動 MQTT
  myComm.begin();        // 初始化通信
  myComm.startListening();  // 啟動監聽
}

void loop() {
  // 處理 WiFi 連線
  if (handleWiFi()) {
      // 處理 MQTT 連線
      if (handleMQTT()) {
          digitalWrite(ledPin, HIGH);  // 若 MQTT 成功連線，亮起 LED
      } else {
          digitalWrite(ledPin, LOW);   // 若 MQTT 斷開，關閉 LED
      }
  } else {
      digitalWrite(ledPin, LOW);       // 若 WiFi 斷開，關閉 LED
  }
  
  myComm.send("SCAN");
  delay(2000);
}
