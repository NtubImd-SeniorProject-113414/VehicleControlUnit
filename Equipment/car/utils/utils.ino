#include <Arduino.h>
#include "BarcodeScanner.h"
#include "ESP32Comm.h"

ESP32Comm myComm(Serial1, 19, 21, 9600);  // 創建通信物件，使用 UART1

void setup() {
  Serial.begin(115200);                      // 初始化監視器串口
  barcodeScannerBegin(16, 17, 9600);         // 初始化條碼掃描器
  myComm.begin();                            // 初始化串口(第一台ESP32)
}

void loop() {
  if (myComm.serial.available()) {
    String received = myComm.serial.readString();
    Serial.println("Received: " + received);

    if (received.indexOf("SCAN") != -1) {
      triggerScan();
      delay(10);
    } else {
      Serial.println("Unknown command: " + received);
    }
  }

  String barcodeData = eventScan();
  if (barcodeData != "") {
    Serial.println("Sending barcode data: " + barcodeData);
    myComm.send(barcodeData);
  }
}
