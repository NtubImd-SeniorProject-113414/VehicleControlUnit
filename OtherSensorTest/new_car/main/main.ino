#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "control.h"
#include "espnow_manager.h"

const int ledPin = 2;
const uint8_t peerMACAddress[] = {0x24, 0x6F, 0x28, 0xB4, 0xDD, 0x99}; // 第二台 ESP32 的 MAC 地址

void setup() {
  Serial.begin(115200);

  initControl();
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  startWiFi();
  startMQTT();
  initESPNow();  // 初始化 ESP-NOW
}

void loop() {
  if (handleWiFi()) {
      if (handleMQTT()) {
          digitalWrite(ledPin, HIGH);

          // 發送一個指令到另一個 ESP32
          const char* message = "Hello from ESP32";
          if (sendCommand(peerMACAddress, (uint8_t*)message, strlen(message))) {
              Serial.println("Message sent successfully");
          } else {
              Serial.println("Failed to send message");
          }

      } else {
          digitalWrite(ledPin, LOW);
      }
  } else {
      digitalWrite(ledPin, LOW);
  }
}
