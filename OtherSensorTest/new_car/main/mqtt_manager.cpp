#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_manager.h"
#include "control.h"

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqttServer = "140.131.115.152";
const int mqttPort = 1883;
const char* mqttUser = "medimate";
const char* mqttPassword = "t;65l454cjo4vu4wj/3";

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

void startMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

bool handleMQTT() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  return client.connected();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      client.subscribe("robot/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 second");
      delay(1000);
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
  } else if (command == "stop") {
    stop();
  }
}
