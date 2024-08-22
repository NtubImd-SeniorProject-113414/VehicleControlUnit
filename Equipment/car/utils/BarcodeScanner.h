#ifndef BARCODE_SCANNER_H
#define BARCODE_SCANNER_H

#include <Arduino.h>
#include <SoftwareSerial.h>

// 函數聲明
void barcodeScannerBegin(int rxPin, int txPin, long baudRate);
void triggerScan();
bool eventScan();
bool isPrintable(char c);
bool isValidData(String data);

#endif
