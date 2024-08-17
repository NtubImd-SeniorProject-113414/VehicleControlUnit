#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "control.h"
#include "espnow_manager.h"

const int ledPin = 2;
const uint8_t peerMACAddress[] = {0xE0, 0x5A, 0x1B, 0xD0, 0xF8, 0x64};

void setup() {
  Serial.begin(115200);

  initControl();
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  String macAddress = WiFi.macAddress();
  Serial.print("ESP32 MAC Address: ");
  Serial.println(macAddress);
  startWiFi();
  startMQTT();
}

void loop() {
  if (handleWiFi()) {
      if (handleMQTT()) {
          digitalWrite(ledPin, HIGH);
          const char* message = "Hello from ESP32";
          if (sendCommand(peerMACAddress, (uint8_t*)message, strlen(message))) {
              Serial.println("Message sent successfully");
          } else {
              
          }
      } else {
          digitalWrite(ledPin, LOW);
      }
  } else {
      digitalWrite(ledPin, LOW);
  }
}
