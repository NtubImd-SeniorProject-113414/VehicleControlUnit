#include "ESP32Comm.h"

const int rxPin = 19;      // RX pin (從其他 ESP32 接收數據)
const int txPin = 21;      // TX pin (傳送數據到其他 ESP32)

ESP32Comm myComm(Serial1, rxPin, txPin, 9600);  // 創建通信物件，使用 UART1

void setup() {
  Serial.begin(115200);  // 用於監視器通訊
  myComm.begin();        // 初始化通信
  myComm.startListening();  // 啟動監聽
}

void loop() {
  myComm.send("ON");
  delay(2000);
}
