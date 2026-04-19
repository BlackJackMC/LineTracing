#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ArduinoJson.h>
#include <vector>

namespace ArduinoJson
{
    template <typename T>
    struct Converter<std::vector<T>>
    {
        static void toJson(const std::vector<T> &src, JsonVariant dst)
        {
            JsonArray array = dst.to<JsonArray>();
            for (T item : src)
            {
                array.add(item);
            }
        }

        static std::vector<T> fromJson(JsonVariantConst src)
        {
            std::vector<T> dst;
            for (JsonVariantConst item : src.as<JsonArrayConst>())
            {
                dst.push_back(item.as<T>());
            }
            return dst;
        }

        static bool checkJson(JsonVariantConst src)
        {
            JsonArrayConst array = src.as<JsonArrayConst>();
            bool result = array;
            for (JsonVariantConst item : array)
            {
                result &= item.is<T>();
            }
            return result;
        }
    };
}

#include "config.h"
#include "motor.h"
#include "qtr_8a.h"
#include "pid.h"
#include "state_manager.h"

AsyncWebServer *server = nullptr;
AsyncWebSocket *ws = nullptr;

Servo servo;
QTR qtr(qtr_pins, qtr_num_sensors);
Motor motor(ENC_A, ENC_B, MOTOR_IN1, MOTOR_IN2, MOTOR_PWM, MOTOR_PPR, MOTOR_GEAR_RATIO);

PIDController speed_controller, qtr_controller;

StateManager state;

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final and info->index == 0 and info->len == len and info->opcode == WS_TEXT)
        {
            JsonDocument incomingDoc;
            if (!deserializeJson(incomingDoc, data, len) and incomingDoc["target"].is<String>())
            {
                String target = incomingDoc["target"].as<String>();
                JsonVariant value = incomingDoc["value"];

                if (state.updateFromWeb(target, value))
                    ws->textAll(state.getJsonString());
            }
        }
    }
}

void setup_hardware()
{
    // ESP32PWM::allocateTimer(1);
    // ESP32PWM::allocateTimer(2);
    // ESP32PWM::allocateTimer(3);
    servo.setPeriodHertz(50);
    servo.attach(SERVO_PIN, 500, 2400);

    qtr.begin();
    motor.begin();

    pinMode(QTR_IR_PIN, OUTPUT);
    digitalWrite(QTR_IR_PIN, HIGH);
}

void setup_wifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(100);
    }
    Serial.println("Connected to wifi");
}

void setup_server()
{
    ws->onEvent(onWsEvent);
    server->addHandler(ws);
    server->begin();
}

void check_wifi()
{
    static unsigned long previousWifiMillis = 0;
    unsigned long currentMillis = millis();

    if ((WiFi.status() != WL_CONNECTED) and (currentMillis - previousWifiMillis >= 10000))
    {
        WiFi.disconnect();
        WiFi.begin(SSID, PASSWORD);
        previousWifiMillis = currentMillis;
    }
}

void register_variable()
{
    state.bindVar(String("test"), &(qtr.output), false);
}

void setup()
{
    Serial.begin(115200);
    long startTime = millis();
    while (!Serial and (millis() - startTime < 3000))
        delay(10);

    register_variable();

    setup_hardware();

    setup_wifi();

    server = new AsyncWebServer(80);
    ws = new AsyncWebSocket("/ws");

    setup_server();

    state.begin(ws, 100);
}

void loop()
{
    check_wifi();
    state.loop();

    qtr.read();

    for (int i = 0; i < qtr_num_sensors; i++)
    {
        Serial.print(analogRead(qtr_pins[i]));
        Serial.print("\t");
    }
    Serial.println();

    delay(1000);
}