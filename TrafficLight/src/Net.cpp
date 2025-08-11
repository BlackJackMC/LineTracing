#include "Net.h"

bool setupESPNow(esp_now_send_cb_t callback)
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
        return false;

    esp_now_register_send_cb(callback);

    esp_now_peer_info_t peer_info;
    memcpy(peer_info.peer_addr, RECEIVER_MAC, 6);
    peer_info.channel = 0;
    peer_info.encrypt = false;

    if (esp_now_add_peer(&peer_info) != ESP_OK)
        return false;

    return true;
}

esp_err_t sendMessage(message_t message)
{
    return esp_now_send(RECEIVER_MAC, (uint8_t *)&message, sizeof(message));
}