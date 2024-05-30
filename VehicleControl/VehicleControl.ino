#include <PubSubClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char* ssid = "BIRC";
const char* password = "D@x6=720131224";
const char* mqtt_server = "140.131.115.152";
const int mqtt_port = 1883;

// L298N
const int ENA = 15; // 黑 
const int ENB = 2;  // 白
const int IN1 = 0;  // 綠
const int IN2 = 4;  // 藍
const int IN3 = 5;  // 紫
const int IN4 = 13; // 灰
// RFID 偵測範圍大約 上及下的 1.5公分
const int MOSI_PIN = 23; // 橘
const int MISO_PIN = 19; // 紅
const int SCK_PIN = 18;  // 白
const int RST_PIN = 32;  // 黃
const int SDA_PIN = 21;  // 銀 VCC=黑 GND=綜
// 循跡
const int rightTrackPin = 39;
const int leftTrackPin = 36;
// 秤重
const int DT_PIN = 14;
const int W_SCK_PIN = 12;
const int scale_factor = 500; //比例參數，從校正程式HX711_Calibration中取得

MFRC522 mfrc522(SDA_PIN, RST_PIN);

enum MotorDirection {
  FORWARD,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  STOP
};

// void setup_wifi() {
//   delay(10);
//   Serial.println("Connecting to WiFi..");
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void setup_mqtt() {
//   client.setServer(mqtt_server, mqtt_port);
//   client.setCallback(callback);
// }

void setup_unit() {
  // pinMode(avoidancePin, INPUT);
  pinMode(rightTrackPin, INPUT);
  pinMode(leftTrackPin, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 120);
  analogWrite(ENB, 120);
  controlMotors(STOP);
  // SPI.begin();                     // 初始化 SPI 介面
  // mfrc522.PCD_Init();              // 初始化 MFRC522
  // Serial.println("請將 RFID 標籤靠近讀取器...");
}

// void callback(char* topic, byte* message, unsigned int length) {
//   Serial.print("Message arrived on topic: ");
//   Serial.print(topic);
//   Serial.print(". Message: ");
//   String messageTemp;
  
//   for (int i = 0; i < length; i++) {
//     messageTemp += (char)message[i];
//   }
//   Serial.println(messageTemp);

//   if (messageTemp == "FORWARD") {
//     controlMotors(FORWARD);
//   } else if (messageTemp == "BACKWARD") {
//     controlMotors(BACKWARD);
//   } else if (messageTemp == "LEFT") {
//     controlMotors(TURN_LEFT);
//   } else if (messageTemp == "RIGHT") {
//     controlMotors(TURN_RIGHT);
//   }
//   delay(1000); // 動作持續 1 秒
//   controlMotors(STOP); // 停止馬達
// }

void reconnect() {
  controlMotors(STOP);

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // 嘗試連接，並傳入用戶名和密碼
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // 一旦連接，訂閱需要的主題
      client.subscribe("motorControl");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 等待 5 秒後再次嘗試
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_unit();
  // setup_wifi();
  // setup_mqtt();
}

void controlMotors(MotorDirection direction) {
  switch (direction) {
      case FORWARD:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;
      case BACKWARD:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          break;
      case TURN_LEFT:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;
      case TURN_RIGHT:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;
      case STOP:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;
    }
}

// void RFID_listening() {
//   if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
//     Serial.print("卡片 UID:");
//     unsigned long uidDec = 0;  // 用來儲存十進制的UID

//     // 打印出十六進制 UID 並轉換為十進制
//     for (byte i = 0; i < mfrc522.uid.size; i++) {
//       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//       Serial.print(mfrc522.uid.uidByte[i], HEX);
//       uidDec = uidDec * 256 + mfrc522.uid.uidByte[i];  // 將十六進制轉換為十進制
//     }
//     Serial.println();

//     // 打印十進制 UID
//     Serial.print("卡片 UID (十進制): ");
//     Serial.println(uidDec);

//     // 讓讀取器準備下一次讀取
//     mfrc522.PICC_HaltA();
//   }
// }

void loop() {
  // RFID_listening()
  // int leftSensorValue = digitalRead(leftTrackPin);
  // int rightSensorValue = digitalRead(rightTrackPin);

  // if (leftSensorValue == 0 && rightSensorValue == 0) {
  //     // 两侧传感器均检测到白色，前进
  //     controlMotors(FORWARD);  
  //     Serial.println("白白全亮前進");
  // } else if (leftSensorValue == 1 && rightSensorValue == 0) {
  //     // 左侧传感器检测到黑色，右侧检测到白色，左转
  //     controlMotors(TURN_LEFT);
  //     Serial.println("黑白左轉");
  // } else if (leftSensorValue == 0 && rightSensorValue == 1) {
  //     // 左侧传感器检测到白色，右侧检测到黑色，右转
  //     controlMotors(TURN_RIGHT);
  //     Serial.println("白黑右轉");

  // } else {
  //     // 其他情况（例如两侧都是黑色），可以选择停止或者定义其他行为
  //     controlMotors(BACKWARD);
  //     Serial.println("黑黑不亮停下");
  // }
  controlMotors(FORWARD);
  delay(3000);
  controlMotors(BACKWARD);
  delay(3000);
}

// void getAvoidanceValue() {
//   int avoidanceValue = digitalRead(avoidancePin);
//   String sensorString = "";

//   if (avoidanceValue == LOW) {
//     Serial.println("Obstacle detected!");
//   } else {
//     Serial.println("No obstacle detected.");
//   }
//   delay(200);
// }
