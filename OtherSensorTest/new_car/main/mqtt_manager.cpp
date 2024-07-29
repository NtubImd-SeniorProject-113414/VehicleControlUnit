#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_manager.h"
#include "control.h"

WiFiClient espClient;
PubSubClient client(espClient);

// 前向声明
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

void startMQTT() {
  client.setServer("MQTT_SERVER_IP", 1883);
  client.setCallback(callback);
}

void handleMQTT() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// 下面是你已经定义的 callback 和 reconnect 函数
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("robot/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  if (command == "forward") {
    moveForward();
  } else if (command == "backward") {
    moveBackward();
  } else if (command == "left") {
    turnLeft();
  } else if (command == "right") {
    turnRight();
  }
}
