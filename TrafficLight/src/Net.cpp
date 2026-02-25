#include "Net.h"
#include "env.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

void wifi_setup()
{
    delay(10);

    Serial.printf("Connecting WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.printf("");
    Serial.printf("WiFi connected");
}

void mqtt_connect()
{
    while (!mqtt_client.connected())
    {
        Serial.printf("Connect MQTT");
        if (mqtt_client.connect(MQTT_ID))
        {
            Serial.printf("Connected");
            // ...
        }
        else 
        {
            Serial.print("Failed. rc="); Serial.print(mqtt_client.state());
            Serial.printf("Retry in 5s");
            delay(5000);
        }
    }
}

void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

esp_now_peer_info peer_info;

void esp_now_setup()
{
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP Now init failed");
        return;
    }

    esp_now_register_send_cb(esp_now_send_cb_t(on_data_sent));

    memcpy(peer_info.peer_addr,RECEIVER_MAC,6);
    peer_info.channel = 0;
    peer_info.encrypt = false;

    if (esp_now_add_peer(&peer_info) != ESP_OK)
    {
        Serial.println("ESP Now peer setup failed");
        return;
    }
}

void send_message(Data data)
{
    esp_err_t res = esp_now_send(RECEIVER_MAC, (uint8_t *)&data, sizeof(data));
    if (result == ESP_OK) {
    Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the data");
    }
}