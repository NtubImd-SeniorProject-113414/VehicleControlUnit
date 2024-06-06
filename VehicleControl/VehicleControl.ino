#include <PubSubClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

// wifi
const char* ssid = "BIRC";
const char* password = "D@x6=720131224";
// const char* ssid = "Hanhan";
// const char* password = "01234566789";
// mqtt
const char* mqtt_server = "broker.MQTTGO.io";
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
// RFID
const int SDA_PIN = 21;  // 橘              
const int SCK_PIN = 18;  // 固定腳位 綠
const int MOSI_PIN = 23; // 固定腳位 棕
const int MISO_PIN = 19; // 固定腳位 黃
const int RST_PIN = 22;  // 紫
// 循跡 D0
const int rightTrackPin = 34;
const int leftTrackPin = 35;
// // 秤重
// const int DT_PIN = 14;
// const int W_SCK_PIN = 12;
// const int scale_factor = 500; //比例參數，從校正程式HX711_Calibration中取得
WiFiClient espClient;
PubSubClient MQTTClient(espClient);
MFRC522 mfrc522(SDA_PIN, RST_PIN);
TimerHandle_t checkConnectTimer;

char* MQTTUser = "";//不須帳密
char* MQTTPassword = "";//不須帳密
String sensor_card = "";
String card_code = "";

enum MotorDirection {
  FORWARD,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  STOP
};

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

void controlMotorsByBlackLine() {
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

void createTask(TaskFunction_t taskFunction, const char* taskName, UBaseType_t priority) {
  xTaskCreate(taskFunction, taskName, 2048, NULL, priority, NULL);
}

// String RFID_listening() {
//   String uidHex = "";
//   if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
//     for (byte i = 0; i < mfrc522.uid.size; i++) {
//       if (mfrc522.uid.uidByte[i] < 0x10) {
//         uidHex += " 0";
//       } else {
//         uidHex += " ";
//       }
//       uidHex += String(mfrc522.uid.uidByte[i], HEX);
//     }
//     mfrc522.PICC_HaltA();
//   }
//   if(uidHex != ""){
//     // Serial.print(uidHex);
//     return uidHex;
//   }
// }

String RFID_listening() {
  String uidHex = "0x";
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) {
        uidHex += "0";
      }
      uidHex += String(mfrc522.uid.uidByte[i], HEX);
    }
    mfrc522.PICC_HaltA();
  }
  return uidHex;
}

void callback(char* topic, byte* message, unsigned int length) {
  String receive_card;
  for (int i = 0; i < length; i++) {
    receive_card += (char)message[i];
  }

  Serial.print("監聽主題: ");
  Serial.println(topic);
  Serial.print("要感應的卡號: ");
  Serial.println(receive_card);

  if (String(topic) == "ntubimd/car/delivery/order") {
    Serial.println("送餐去!");
    deliveringOrder(receive_card);
  }

  if (String(topic) == "ntubimd/car/delivery/medicine") {
    // Serial.println("送餐去!");
    deliveringMedicine(receive_card);
  }
}

void deliveringOrder(String receive_card) {
  while (true) {
    String sensor_card = RFID_listening();
    
    if (sensor_card != "") {
      if (sensor_card == receive_card) {
        controlMotors(STOP);
        MQTTClient.publish("ntubimd/order/destination", sensor_card.c_str());
        break;
      } else {
        controlMotorsByBlackLine();
      }
    }
    delay(500);
  }
}

// task 任務區塊
void deliveringMedicine(String receive_card) {
  while (true) {
    String sensor_card = RFID_listening();
    
    if (sensor_card != "") {
      if (sensor_card == receive_card) {
        controlMotors(STOP);
        MQTTClient.publish("ntubimd/medicine/destination", sensor_card.c_str());
        break;
      } else {
        controlMotorsByBlackLine();
      }
    }
    delay(500);
  }
}

void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println(WiFi.localIP());
    delay(500);
  }
  Serial.println("Wifi ok");
}

void connectMqtt() {
  while (!MQTTClient.connected()) {
    String  MQTTClientid = "esp32-" + String(random(1000000, 9999999));//建立一個id，MQTT要求id要不一樣，所以使用亂數
    if (MQTTClient.connect("ntubimd-esp32-car-dwkoejfihfug", MQTTUser, MQTTPassword)) {
      Serial.println("connected");
      MQTTClient.subscribe("ntubimd/car/delivery/order");
      MQTTClient.subscribe("ntubimd/car/delivery/medicine");
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTClient.state());
      Serial.println(" try again in 5 seconds");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }

  Serial.println("Mqtt ok");
}

void checkConnection(TimerHandle_t xTimer) {
  while(1){
    if (WiFi.status() == WL_CONNECTED && MQTTClient.connected()) {
      Serial.println("status ok");
      MQTTClient.publish("ntubimd/car/status", "1");
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
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
  MQTTClient.setServer(mqtt_server, 1883);
  MQTTClient.setCallback(callback);
  createTask(checkConnection, "checkConnection", 5);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) { connectWiFi(); }
  if (!MQTTClient.connected()) {
    connectMqtt();
  }
  MQTTClient.loop();
}
