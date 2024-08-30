#include <Arduino.h>
#include "BarcodeScanner.h"
#include "ESP32Comm.h"

const int sensor1Pin = 33;
const int sensor2Pin = 25;
const int sensor3Pin = 26;
const int sensor4Pin = 27;

bool state = false;

ESP32Comm myComm(Serial1, 19, 21, 9600);

void setup() {
  Serial.begin(115200);
  barcodeScannerBegin(16, 17, 9600);
  myComm.begin();

  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
}

void loop() {
  int sensor1State = digitalRead(sensor1Pin);
  int sensor2State = digitalRead(sensor2Pin);
  int sensor3State = digitalRead(sensor3Pin);
  int sensor4State = digitalRead(sensor4Pin);

  if (myComm.serial.available()) {
    String received = myComm.serial.readStringUntil('\n');
    Serial.println("Received: " + received);

    if (received.indexOf("SCAN") != -1) {
      triggerScan();
    } else {
      Serial.println("Unknown command: " + received);
    }
  }

  String barcodeData = eventScan();
  if (barcodeData != "") {
    Serial.println("Sending barcode data: " + barcodeData);
    myComm.send(barcodeData);
  }

  if(sensor1State == 1 && sensor4State == 1){
    state = true;
    Serial.println("Test");
  } else if(sensor1State == 1){
    myComm.send("OFFSET_RIGHT");
    state = true;
    Serial.println("Detected non-black surface, sending OFFSET_RIGHT command.");
  } else if(sensor4State == 1){
    myComm.send("OFFSET_LEFT");
    state = true;
    Serial.println("Detected non-black surface, sending OFFSET_LEFT command.");
  } else if (sensor2State == 0 && sensor3State == 0) {
    myComm.send("STOP");
    state = true;
    Serial.println("Detected non-black surface, sending STOP command.");
  } else {
    if(state){
      state = false;
      myComm.send("FORWARD");
      Serial.println("Detected non-black surface, sending FORWARD command.");
    }
  }
  delay(100);
}
