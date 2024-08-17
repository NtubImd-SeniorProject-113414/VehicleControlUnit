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

    // 檢查並打印具體的錯誤代碼含義
    if (result == ESP_OK) {
        Serial.println("Message sent successfully");
    } else {
        Serial.print("Error sending message: ");
        switch (result) {
            case ESP_ERR_ESPNOW_NOT_INIT:
                Serial.println("ESPNOW not initialized");
                break;
            case ESP_ERR_ESPNOW_ARG:
                Serial.println("Invalid argument");
                break;
            case ESP_ERR_ESPNOW_INTERNAL:
                Serial.println("Internal error");
                break;
            case ESP_ERR_ESPNOW_NO_MEM:
                Serial.println("Out of memory");
                break;
            case ESP_ERR_ESPNOW_NOT_FOUND:
                Serial.println("Peer not found");
                break;
            case ESP_ERR_ESPNOW_IF:
                Serial.println("Interface error");
                break;
            default:
                Serial.println("Unknown error");
                break;
        }
    }

    return result == ESP_OK;
}

void addPeer(const uint8_t *peer_addr) {
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(esp_now_peer_info_t));  // 清空內存
    memcpy(peerInfo.peer_addr, peer_addr, 6);           // 將對等設備的 MAC 地址複製進去
    peerInfo.channel = 0;                               // 使用預設的 Wi-Fi 通道（0 表示自動選擇）
    peerInfo.encrypt = false;                           // 不加密

    // 添加對等設備
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
    } else {
        Serial.println("Peer added successfully");
    }
}

void registerReceiveCallback(void (*recvCallback)(const uint8_t *mac_addr, const uint8_t *data, int data_len)) {
    esp_now_register_recv_cb(recvCallback);
}
