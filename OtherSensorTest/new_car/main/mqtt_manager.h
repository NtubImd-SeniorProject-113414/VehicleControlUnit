#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

void startMQTT();
void handleMQTT();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

#endif
