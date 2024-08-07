#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "control.h"

const int ledPin = 2;

void setup() {
  Serial.begin(115200);

  initControl();
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  startWiFi();
  startMQTT();
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
}

