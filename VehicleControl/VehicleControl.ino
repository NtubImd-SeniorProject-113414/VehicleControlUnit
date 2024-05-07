#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "BIRC";
const char* password = "D@x6=720131224";
const char* mqtt_server = "140.131.115.152";
const int mqtt_port = 1883;

// 馬達控制腳位
const int ENA = 15; // 黑 
const int ENB = 2;  // 白
const int IN1 = 0;  // 綠
const int IN2 = 4;  // 藍
const int IN3 = 5;  // 紫
const int IN4 = 18; // 灰
const int AvoidancePin = 7;
const int RightTrackPin = 19; // TCRT5000的D0接到数字脚位D4 右邊
const int LeftTrackPin = 21; // TCRT5000的D0接到数字脚位D4 左邊

enum MotorDirection {
  FORWARD,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  STOP
};

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_mqtt() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void setup_unit() {
  pinMode(AvoidancePin, INPUT);
  pinMode(RightDigitalPin, INPUT);
  pinMode(LeftDigitaPin, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 120);
  analogWrite(ENB, 120);
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  if (messageTemp == "FORWARD") {
    forward();
  } else if (messageTemp == "BACKWARD") {
    backward();
  } else if (messageTemp == "LEFT") {
    turnLeft();
  } else if (messageTemp == "RIGHT") {
    turnRight();
  }
  delay(1000); // 動作持續 1 秒
  stopMotors(); // 停止馬達
}

void reconnect() {
  controlMotors(STOP)

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
  setup_wifi();
  setup_mqtt();
  setup_unit()
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

void loop() {
  int leftSensorValue = digitalRead(leftSensorPin);
  int rightSensorValue = digitalRead(rightSensorPin);

  if (leftSensorValue == 0 && rightSensorValue == 0) {
      // 两侧传感器均检测到白色，前进
      controlMotors(FORWARD);
  } else if (leftSensorValue == 1 && rightSensorValue == 0) {
      // 左侧传感器检测到黑色，右侧检测到白色，左转
      controlMotors(TURN_LEFT);
  } else if (leftSensorValue == 0 && rightSensorValue == 1) {
      // 左侧传感器检测到白色，右侧检测到黑色，右转
      controlMotors(TURN_RIGHT);
  } else {
      // 其他情况（例如两侧都是黑色），可以选择停止或者定义其他行为
      controlMotors(STOP);
  }
}

void getAvoidanceValue() {
  int avoidanceValue = digitalRead(AvoidancePin);
  String sensorString = "";

  if (sensorValue == LOW) {
    Serial.println("Obstacle detected!");
  } else {
    Serial.println("No obstacle detected.");
  }
  delay(200);
}
