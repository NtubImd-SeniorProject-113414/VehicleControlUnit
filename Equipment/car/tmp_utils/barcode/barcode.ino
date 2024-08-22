#include <SoftwareSerial.h>

// 定義條碼掃描器的串口引腳
SoftwareSerial barcodeScanner(16, 17); // RX, TX

void setup() {
  Serial.begin(9600);  // Arduino串口監視器的波特率
  barcodeScanner.begin(9600); // 條碼掃描器的波特率（通常是9600）

  // 初始化設置完成後，可以立即觸發一次掃描
  triggerScan();
}

void loop() {
  eventScan();
}

// 發送觸發掃描的命令到條碼掃描器
void triggerScan() {
  byte command[] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};
  barcodeScanner.write(command, sizeof(command));
}

void eventScan() {
  if (barcodeScanner.available()) {
    String barcodeData = "";
    bool TF = false;

    while (barcodeScanner.available()) {
      char c = barcodeScanner.read();
      if (isPrintable(c)) {  // 僅拼接可打印字符
        barcodeData += c;
      } else {
        TF = true;
      }
      delay(10);
    }

    // 替代條件檢查，例如根據數據長度或其他特定條件進行判斷
    if (barcodeData.length() > 0 && isValidData(barcodeData)) {
      if (TF) {
        int index = barcodeData.indexOf("31");
        if (index != -1) {
          barcodeData.remove(index, 2); // 移除第一次出現的 "31"
        }
      }
      Serial.println("Barcode Data: " + barcodeData);
      triggerScan();
    }
  }
}

// 僅允許ASCII 32到126之間的可打印字符
bool isPrintable(char c) {
  return c >= 32 && c <= 126;
}

// 根據條件過濾無效數據
bool isValidData(String data) {
  // 在這裡定義你認為有效數據的條件
  // 例如，根據長度、格式等進行過濾
  // 現在假設所有長度大於2的數據都是有效的
  return data.length() > 2;
}
