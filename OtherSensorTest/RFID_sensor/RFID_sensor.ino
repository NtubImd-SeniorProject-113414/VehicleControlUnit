#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>


// RFID
const int MOSI_PIN = 23; // 橘
const int MISO_PIN = 19; // 紅
const int SCK_PIN = 18;  // 白
const int RST_PIN = 32;  // 黃
const int SDA_PIN = 21;  // 銀

MFRC522 mfrc522(SDA_PIN, RST_PIN);
void setup_unit() {
  SPI.begin();                     // 初始化 SPI 介面
  mfrc522.PCD_Init();              // 初始化 MFRC522
  Serial.println("請將 RFID 標籤靠近讀取器...");
}

void setup() {
  Serial.begin(9600);
  setup_unit();
}

void RFID_listening() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("卡片 UID:");
    unsigned long uidDec = 0;  // 用來儲存十進制的UID

    // 打印出十六進制 UID 並轉換為十進制
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      uidDec = uidDec * 256 + mfrc522.uid.uidByte[i];  // 將十六進制轉換為十進制
    }
    Serial.println();

    // 打印十進制 UID
    Serial.print("卡片 UID (十進制): ");
    Serial.println(uidDec);

    // 讓讀取器準備下一次讀取
    mfrc522.PICC_HaltA();
  }
}

void loop() {
  RFID_listening();
}

