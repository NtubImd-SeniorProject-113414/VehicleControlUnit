#include "ESP32Comm.h"

ESP32Comm::ESP32Comm(HardwareSerial& serialPort, int rxPin, int txPin, long baudRate)
  : serial(serialPort), rxPin(rxPin), txPin(txPin), baudRate(baudRate) {}

void ESP32Comm::begin() {
  serial.begin(baudRate, SERIAL_8N1, rxPin, txPin);  // 初始化 UART
}

void ESP32Comm::send(const String& message) {
  serial.print(message);  // 發送消息
}

void ESP32Comm::startListening() {
  xTaskCreatePinnedToCore(listenTask, "ListenTask", 2048, this, 1, NULL, 1);  // 將任務綁定到核心 1
}

// 用於處理串口訊息的多執行緒函數
void ESP32Comm::listenTask(void* param) {
  ESP32Comm* comm = static_cast<ESP32Comm*>(param);  // 轉換參數為 ESP32Comm 類型

  while (true) {
    if (comm->serial.available()) {
      String received = comm->serial.readString();  // 讀取串口資料
      Serial.println(received);
    }

    vTaskDelay(25 / portTICK_PERIOD_MS);  // 延遲 100 毫秒以避免佔用太多 CPU
  }
}
