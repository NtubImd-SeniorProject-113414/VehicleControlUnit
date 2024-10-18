#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_manager.h"
#include "control.h"
#include "ESP32Comm.h"

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqttServer = "140.131.115.42";
const int mqttPort = 1883;
const char* mqttUser = "medimate";
const char* mqttPassword = "t;65l4u xul6vu4wj/3";

extern ESP32Comm myComm;

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

  // 處理不同的命令
  if (command.indexOf("speed") != -1) {
    setSpeed(command.substring(command.indexOf(",") + 1).toInt());
  }else if (command == "forward") {
    moveForward();
    myComm.send("ON");
  } else if (command == "backward") {
    moveBackward();
  } else if (command == "left") {
    turnLeft();
  } else if (command == "right") {
    turnRight();
  } else if (command == "shiftLeft") {
    shiftLeft();
  } else if (command == "shiftRight") {
    shiftRight();
  } else if (command == "stop") {
    myComm.send("OFF");
    stop();
  } else if (command == "scan") {
    myComm.send("SCAN");
  } else if (command == "get/mac") {
    String macAddress = WiFi.macAddress();
    client.publish("return/mac", macAddress.c_str());
  }
}
