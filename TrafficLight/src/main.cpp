#include <Arduino.h>
#include "TrafficControl.h"
#include "DistanceSensor.h"
#include "Net.h"

TrafficControl::TaskScheduler light1, light2;

bool one = false;

void onMessageSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("ESPNow send status: ");
    Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    setupDistance(8, 9);
    setupESPNow(onMessageSent);
    // bool tmp[3] = {0, 0, 0};
    // pinMode(5, OUTPUT);
    // pinMode(6, OUTPUT);
    // pinMode(7, OUTPUT);
    // light1.init(0, millis(), TrafficLight::TrafficLight(5, 6, 7, tmp));
    // light2.init()
}

void loop()
{
    getDistance();
    // if (light1.update())
    //     Serial.println(light1.getCurrentState());
    // light2.update();
}
