#pragma once
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "env.h"

struct message_t
{
};

bool setupESPNow(esp_now_send_cb_t callback);
esp_err_t sendMessage(message_t message);
