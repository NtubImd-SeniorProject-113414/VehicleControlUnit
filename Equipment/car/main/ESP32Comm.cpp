#include "ESP32Comm.h"

ESP32Comm::ESP32Comm(HardwareSerial& serialPort, int rxPin, int txPin, long baudRate)
  : serial(serialPort), rxPin(rxPin), txPin(txPin), baudRate(baudRate) {}

void ESP32Comm::begin() {
  serial.begin(baudRate, SERIAL_8N1, rxPin, txPin);  // 初始化 UART
}

void ESP32Comm::send(const String& message) {
  serial.println(message);  // 發送消息
}