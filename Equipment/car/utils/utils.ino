#include <Arduino.h>
#include "BarcodeScanner.h"
#include "ESP32Comm.h"

// 定義紅外線循跡傳感器的 GPIO 腳位
const int sensor1Pin = 33;
const int sensor2Pin = 25;
const int sensor3Pin = 26;
const int sensor4Pin = 27;

// 創建通信物件，使用 UART1
ESP32Comm myComm(Serial1, 19, 21, 9600);

void setup() {
  Serial.begin(115200);                      // 初始化監視器串口
  barcodeScannerBegin(16, 17, 9600);         // 初始化條碼掃描器
  myComm.begin();                            // 初始化串口(第一台ESP32)

  // 初始化紅外線傳感器腳位為輸入
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
}

void loop() {
  // 讀取每個紅外線傳感器的數字輸出
  int sensor1State = digitalRead(sensor1Pin);
  int sensor2State = digitalRead(sensor2Pin);
  int sensor3State = digitalRead(sensor3Pin);
  int sensor4State = digitalRead(sensor4Pin);

  // 如果任何一個傳感器偵測到非黑色（即高反射率），發送 "STOP" 指令
  if (sensor1State == LOW || sensor2State == LOW || sensor3State == LOW || sensor4State == LOW) {
    Serial.println("Detected non-black surface, sending STOP command.");
    myComm.send("STOP");  // 通過通信對象發送 STOP 指令
    delay(1000);  // 為了防止多次發送，延遲1秒
  }

  // 處理接收到的數據（來自另一台ESP32的通信）
  if (myComm.serial.available()) {
    String received = myComm.serial.readString();
    Serial.println("Received: " + received);

    if (received.indexOf("SCAN") != -1) {
      triggerScan();  // 如果收到 SCAN 指令，觸發條碼掃描
      delay(10);
    } else {
      Serial.println("Unknown command: " + received);
    }
  }

  // 檢查是否有條碼數據可用
  String barcodeData = eventScan();
  if (barcodeData != "") {
    Serial.println("Sending barcode data: " + barcodeData);
    myComm.send(barcodeData);  // 發送條碼數據
  }
  delay(5);
}
