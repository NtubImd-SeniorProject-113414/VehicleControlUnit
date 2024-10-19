#include "BarcodeScanner.h"

SoftwareSerial barcodeScanner(16, 17);  // 使用預設 RX = GPIO16, TX = GPIO17

// 初始化條碼掃描器
void barcodeScannerBegin(int rxPin, int txPin, long baudRate) {
  barcodeScanner.begin(baudRate);
}

// 觸發條碼掃描
void triggerScan() {
  byte command[] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};
  barcodeScanner.write(command, sizeof(command));
}

// 處理掃描事件，返回是否有掃描數據
String eventScan() {
  if (barcodeScanner.available()) {
    String barcodeData = "";
    bool TF = false;

    while (barcodeScanner.available()) {
      char c = barcodeScanner.read();
      if (isPrintable(c)) {
        barcodeData += c;  // 僅拼接可打印字符
      } else {
        TF = true;  // 記錄非可打印字符
      }
      delay(10);
    }

    // 檢查數據有效性
    if (barcodeData.length() > 0 && isValidData(barcodeData)) {
      if (TF) {
        int index = barcodeData.indexOf("31");
        if (index != -1) {
          barcodeData.remove(index, 2);
        }
      }
      return barcodeData;
    }
  }
  return "";
}

// 判斷字符是否可打印
bool isPrintable(char c) {
  return c >= 32 && c <= 126;
}

// 檢查條碼數據是否有效
bool isValidData(String data) {
  return data.length() > 2;
}
