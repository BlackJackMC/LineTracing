#pragma once
#include "env.h"

struct Data {
    int light_id;
    int light_status;
};

void wifi_setup();

void mqtt_connect();

void esp_now_setup();

void send_message();