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
PubSubClient client(espClient);
MFRC522 mfrc522(SDA_PIN, RST_PIN);
TimerHandle_t checkConnectTimer;


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

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  Serial.print("監聽主題: ");
  Serial.println(topic);
  Serial.print("要感應的卡號: ");
  Serial.println(messageTemp);

  if (String(topic) == "ntubimd/car/delivering/order") {
    createTask(deliveringOrder, "delivering order", 2);
  }

  if (String(topic) == "ntubimd/car/delivering/medicine") {
    createTask(deliveringMedicine, "delivering medicine", 3);
  }
}


// task 任務區塊
void deliveringMedicine(void *pvParameters) {
  while (sensor_card == "") {
    card_code = RFID_listening();
    if (sensor_card != card_code) {
      controlMotorsByBlackLine();
    } else {
      controlMotors(STOP);
      client.publish("ntubimd/medicine/destination", sensor_card.c_str());
      sensor_card = "";
      card_code = "";
      vTaskDelete(NULL);
    }
  }
}

void deliveringOrder(void *pvParameters) {
  while (sensor_card == "") {
    card_code = RFID_listening();
    if (sensor_card != card_code) {
      controlMotorsByBlackLine();
    } else {
      controlMotors(STOP);
      client.publish("ntubimd/order/destination", sensor_card.c_str());
      sensor_card = "";
      card_code = "";
      vTaskDelete(NULL);
    }
  }
}

void connectWiFi(void *pvParameters) {
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println(WiFi.localIP());
    delay(500);
  }
  Serial.println("Wifi ok");
  vTaskDelete(NULL);
}

void connectMqtt(void *pvParameters) {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("ntubimd/car/delivering/order");
      client.subscribe("ntubimd/car/delivering/medicine");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }
  Serial.println("Mqtt ok");
  vTaskDelete(NULL);
}

void checkConnection(TimerHandle_t xTimer) {
  if (WiFi.status() != WL_CONNECTED) {
    createTask(connectWiFi, "connectWiFi", 0);
  }
  if (WiFi.status() != WL_CONNECTED && !client.connected()){
    createTask(connectMqtt, "connectWiFi", 1);
  }

  if (WiFi.status() == WL_CONNECTED && client.connected()) {
    client.publish("ntubimd/car/status", "1");
    // vTaskDelete(NULL);
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

  checkConnectTimer = xTimerCreate("MyTimer", pdMS_TO_TICKS(10000), pdTRUE, (void*)0, checkConnection);
  xTimerStart(checkConnectTimer, 0);
}

void loop() {
  client.loop();
}
