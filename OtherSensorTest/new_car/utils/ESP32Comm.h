#ifndef ESP32COMM_H
#define ESP32COMM_H

#include <Arduino.h>
#include <HardwareSerial.h>

class ESP32Comm {
  public:
    ESP32Comm(HardwareSerial& serialPort, int rxPin, int txPin, long baudRate);
    void begin();            // 初始化串口
    void send(const String& message);  // 發送訊息
    void startListening();   // 開始監聽訊息 (多執行緒)
  
  private:
    static void listenTask(void* param);  // 用於多執行緒的監聽任務
    HardwareSerial& serial;  // 硬體串口
    int rxPin, txPin;        // RX 和 TX 引腳
    long baudRate;           // 波特率
};

#endif