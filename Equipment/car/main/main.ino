#include <WiFi.h>
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

String receivedData = "";  // Used to store incoming data
String lastCommand = "";   // Used to store the last executed command
bool commandComplete = false;  // Flag to indicate if a full command has been received

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
    char inChar = (char)myComm.serial.read();
    if (isalnum(inChar) || inChar == '-' || inChar == '_') {
      receivedData += inChar;
    }
    if (inChar == '\n') {
      commandComplete = true;
    }
  }

  if (commandComplete) {
    // Handle the command
    if(receivedData.indexOf("FORWARD") != -1){
      moveForward();
    } else if (receivedData.indexOf("STOP") != -1) {
      stop();
    } else if (receivedData.indexOf("OFFSET_RIGHT") != -1) {
      correctRight();
    } else if (receivedData.indexOf("OFFSET_LEFT") != -1) {
      correctLeft();
    } else if(receivedData.indexOf("SLOW") != -1) {
      setSpeed(22);
      moveForward();
    } else {
      String data = WiFi.macAddress() + "," + receivedData;
      client.publish("return/qrcode", data.c_str());
    }
  // Reset the command buffer and flag
    commandComplete = false;
    receivedData = "";
  }
}
