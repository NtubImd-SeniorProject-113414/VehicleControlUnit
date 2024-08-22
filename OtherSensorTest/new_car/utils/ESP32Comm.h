#ifndef ESP32_COMM_H
#define ESP32_COMM_H

#include <HardwareSerial.h>

class ESP32Comm {
  public:
    ESP32Comm(HardwareSerial& serialPort, int rxPin, int txPin, long baudRate);
    void begin();
    void send(const String& message);
    HardwareSerial& serial;

  private:
    int rxPin, txPin;
    long baudRate;
};

#endif
