#include "net.h"
#include "env.h"

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebSocketsClient.h>
#include <esp_now.h>

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

void wifi_setup()
{
    delay(10);
    
    WiFi.mode(WIFI_AP);
    Serial.printf("Broadcasting WiFi");
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    
    // WiFi.mode(WIFI_STA);
    // Serial.printf("Connecting WiFi");
    // WiFi.begin(WIFI_SSID, WIFI_PASS);

    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     Serial.print(".");
    //     delay(500);
    // }
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

//======================================================//

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

//======================================================//

// WebSocketsClient websocket;

// void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
//     switch(type) {
//         case WStype_DISCONNECTED:
//             Serial.printf("[WebSocket] Disconnected!\n");
//             break;
            
//         case WStype_CONNECTED:
//             Serial.printf("[WebSocket] Connected to url: %s\n", payload);
            
//             // Send a test message to the server once connected
//             webSocket.sendTXT("Hello Server, this is ESP32!");
//             break;
//     }
// }




