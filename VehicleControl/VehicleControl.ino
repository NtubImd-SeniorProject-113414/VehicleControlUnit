#include <PubSubClient.h>
#include <WiFi.h>

// WiFi 設定
const char* ssid = "BIRC";
const char* password = "D@x6=720131224";

// MQTT 伺服器設定
const char* mqtt_server = "140.131.115.152";
const int mqtt_port = 1883;

// 馬達控制腳位
const int ENA = 15; 
const int IN1 = 0; 
const int IN2 = 4; 
const int IN3 = 5; 
const int IN4 = 18; 
const int ENB = 2; 

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
  // 確保沒有連接時嘗試連接
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
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 120);
  analogWrite(ENB, 120);
}

void loop() {
  forward();
  delay(5000);
  turnLeft();
  delay(1000);
  backward();
  delay(5000);
  turnRight();
  delay(1000);

  
//  if (!client.connected()) {
//    reconnect();
//  }
//  client.loop();
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
