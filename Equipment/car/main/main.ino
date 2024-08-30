#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "ESP32Comm.h"
#include "control.h"
#include <HardwareSerial.h>

HardwareSerial MySerial(1);

const int rxPin = 35;
const int txPin = 12;
const int ledPin = 2;

ESP32Comm myComm(Serial1, rxPin, txPin, 9600);

String receivedData = "";  // 用來存儲逐字接收的資料
bool commandComplete = false;  // 用來標記是否收到完整指令

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600, SERIAL_8N1, rxPin, txPin);

  initControl();
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  startWiFi();
  startMQTT();
  myComm.begin();
}

void loop() {
  if (handleWiFi()) {
      if (handleMQTT()) {
          digitalWrite(ledPin, HIGH);
      } else {
          digitalWrite(ledPin, LOW);
      }
  } else {
      digitalWrite(ledPin, LOW);
  }

  while (myComm.serial.available() > 0) {
    char inChar = (char)myComm.serial.read();  // 讀取一個字元
    receivedData += inChar;

    if (inChar == '\n') {  // 檢查是否收到換行符
      commandComplete = true;
    }
  }

  if (commandComplete) {
    Serial.println("Received: " + receivedData);

    if (receivedData.indexOf("STOP") != -1) {
      stop();
    } else if (receivedData.indexOf("OFFSET_RIGHT") != -1) {
      offsetRight();
    } else if (receivedData.indexOf("OFFSET_LEFT") != -1) {
      offsetLeft();
    } else if (receivedData.indexOf("FORWARD") != -1) {
      moveForward();
    }

    // 清除接收數據並重置標記
    receivedData = "";
    commandComplete = false;
  }
}
