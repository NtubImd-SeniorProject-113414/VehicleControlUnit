#include <Arduino.h>
#include "BarcodeScanner.h"
#include "ESP32Comm.h"

//onst int sensor1Pin = 33;
const int sensor2Pin = 25;
const int sensor3Pin = 26;
//const int sensor4Pin = 27;

bool state = false;

ESP32Comm myComm(Serial1, 19, 21, 9600);

void setup() {
  Serial.begin(115200);
  barcodeScannerBegin(16, 17, 9600);
  myComm.begin();

  //pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  //pinMode(sensor4Pin, INPUT);
}

void loop() {
  //int sensor1State = digitalRead(sensor1Pin);
  int sensor2State = digitalRead(sensor2Pin);
  int sensor3State = digitalRead(sensor3Pin);
  //int sensor4State = digitalRead(sensor4Pin);

  if (myComm.serial.available()) {
    String received = myComm.serial.readStringUntil('\n');
    Serial.println("Received: " + received);

    if (received.indexOf("SCAN") != -1) {
      triggerScan();
    }else if (received.indexOf("ON") != -1) {
      state = true;
    }else if (received.indexOf("OFF") != -1) {
      state = false;
    } else {
      Serial.println("Unknown command: " + received);
    }
  }

  String barcodeData = eventScan();
  if (barcodeData != "") {
    Serial.println("Sending barcode data: " + barcodeData);
    myComm.send(barcodeData);
  }
  if(state){
    if (sensor2State == 1) {
      myComm.send("OFFSET_RIGHT");
      Serial.println("sending OFFSET_RIGHT command.");
    } else if (sensor3State == 1) {
      myComm.send("OFFSET_LEFT");
      Serial.println("sending OFFSET_LEFT command.");
    }
    delay(100);
  }
}