#include "espnow_manager.h"

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    Serial.print("Received Data from: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(mac_addr[i], HEX);
        if (i < 5) Serial.print(":");
    }
    Serial.print(" | Data: ");
    Serial.write(data, data_len);
    Serial.println();
}

void initESPNow() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);
}

bool sendCommand(const uint8_t *peer_addr, const uint8_t *data, size_t len) {
    esp_err_t result = esp_now_send(peer_addr, data, len);
    return result == ESP_OK;
}

void registerReceiveCallback(void (*recvCallback)(const uint8_t *mac_addr, const uint8_t *data, int data_len)) {
    esp_now_register_recv_cb(recvCallback);
}
