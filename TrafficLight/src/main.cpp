#include <Arduino.h>
#include "traffic_control.h"
#include "distance_sensor.h"
#include "net.h"
#include "global.h"

#include <WebSocketsClient.h>
#include <ArduinoJson.h>

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WebSocket] Disconnected!\n");
            break;
            
        case WStype_CONNECTED:
            Serial.printf("[WebSocket] Connected to url: %s\n", payload);
            
            // Send a test message to the server once connected
            webSocket.sendTXT("Hello Server, this is ESP32!");
            break;
            
        case WStype_TEXT:
            // This triggers when the ESP32 receives a text message from the server
            Serial.printf("[WebSocket] Received text: %s\n", payload);
            break;
            
        case WStype_BIN:
            Serial.printf("[WebSocket] Received binary data, length: %u\n", length);
            break;
            
        case WStype_PING:
            // The library automatically handles PING/PONG, but you can log it here
            Serial.printf("[WebSocket] Ping received\n");
            break;
            
        case WStype_PONG:
            Serial.printf("[WebSocket] Pong received\n");
            break;
    }
}

void send_message() {
    Data msg;
    if (xQueueReceive(msgQueue, &msg, 0) == pdTRUE) {
        JsonDocument doc;

        doc["target"] = (String(msg.type == 'l' ? "light_" : "dist_") + String(msg.isSouth ? "south" : "west")).c_str();
        doc["value"] = String(msg.value);

        String jsonPayload;
        serializeJson(doc, jsonPayload);

        webSocket.sendTXT(jsonPayload);
        
        Serial.print("Sent via WS: ");
        Serial.println(jsonPayload);
    }
}

int prevTime;
const int DISTANCE_DELAY = 1000;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    // setupDistance(0,33,32);
    // setupDistance(1,0,4);
    wifi_setup();

    msgQueue = xQueueCreate(20, sizeof(Data));

    webSocket.beginSSL(ws_host, ws_port, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
    
    prevTime = millis();

    startTL();
}
    
void loop()
{
    webSocket.loop();

    // int curTime = millis();
    // if (curTime - prevTime > DISTANCE_DELAY) {
    //     long dis = getDistance(1);
    //     Serial.print("Distance is ");
    //     Serial.println(dis);
    //     prevTime = curTime;
    // }

    send_message();
}
