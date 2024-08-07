#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

void startMQTT();
bool handleMQTT();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

#endif
