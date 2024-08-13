#ifndef ESPNOW_MANAGER_H
#define ESPNOW_MANAGER_H

#include <esp_now.h>
#include <WiFi.h>

// 定義發送和接收的回調函數
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

// 初始化 ESP-NOW
void initESPNow();

// 發送指令
bool sendCommand(const uint8_t *peer_addr, const uint8_t *data, size_t len);

// 設置接收的回調函數
void registerReceiveCallback(void (*recvCallback)(const uint8_t *mac_addr, const uint8_t *data, int data_len));

#endif