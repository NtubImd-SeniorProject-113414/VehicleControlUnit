#include <PubSubClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

WiFiClient espClient;
PubSubClient client(espClient);
// wifi
const char* ssid = "BIRC";
const char* password = "D@x6=720131224";
// mqtt
const char* mqtt_server = "broker.mqttgo.io";
const int mqtt_port = 1883;

// L298N 藍紫灰白黑棕
const int ENA = 15; // 藍 
const int IN1 = 2;  // 
const int IN2 = 0;  // 藍
const int IN3 = 4;  // 紫
const int IN4 = 16; // 灰
const int ENB = 17;  // 白
// RFID 偵測範圍大約 上及下的 1.5公分
// 由左到右
// 接在esp的順序 綠黃橘紫棕
// 接在rfid的順序   
const int SDA_PIN = 21;  // 綠              
const int SCK_PIN = 18;  // 固定腳位 紫
const int MOSI_PIN = 23; // 固定腳位 橘
const int MISO_PIN = 19; // 固定腳位 藍
const int RST_PIN = 22;  // 黃
// 循跡 D0
const int rightTrackPin = 34;
const int leftTrackPin = 35;
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

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void setup_mqtt() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.subscribe("ntubimd/delivering");
}

void setup_unit() {
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
  SPI.begin();
  mfrc522.PCD_Init();
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  Serial.print("監聽主題: ");
  Serial.println(topic);
  Serial.print("要感應的卡號: ");
  Serial.println(messageTemp);

  if (String(topic) == "ntubimd/delivering/order") {
    String sensor_card = delieveryUtilGetCardCode(messageTemp);
    client.publish("ntubimd/already/delivery/order", sensor_card.c_str());
  }

  if (String(topic) == "ntubimd/delivering/medicine") {
    String sensor_card = delieveryUtilGetCardCode(messageTemp);
    client.publish("ntubimd/already/delivery/medicine", sensor_card.c_str());    
  }
}

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
  setup_wifi();
  setup_mqtt();
}

void loop() {
  RFID_listening()
}

String delieveryUtilGetCardCode(String card_code){
  String sensor_card = RFID_listening();
  controlMotorsByBlackLine();
  if (sensor_card != card_code) {
    controlMotors(STOP);
  }
  return sensor_card;
}

void controlMorosByBlackLine() {
  int leftSensorValue = digitalRead(leftTrackPin);
  int rightSensorValue = digitalRead(rightTrackPin);

  if (leftSensorValue == 0 && rightSensorValue == 0) {
    controlMotors(FORWARD);
  } else if (leftSensorValue == 1 && rightSensorValue == 0) {
    controlMotors(TURN_LEFT);
  } else if (leftSensorValue == 0 && rightSensorValue == 1) {
    controlMotors(TURN_RIGHT);
  } else {
    controlMotors(BACKWARD);
  }
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

String RFID_listening() {
  String uidHex = "";
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) {
        uidHex += " 0";
      } else {
        uidHex += " ";
      }
      uidHex += String(mfrc522.uid.uidByte[i], HEX);
    }
    mfrc522.PICC_HaltA();
  }
  return uidHex;
}