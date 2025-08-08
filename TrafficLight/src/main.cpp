#include <Arduino.h>
#include "trafficControl.h"

TrafficControl::TaskScheduler light1, light2;

bool one = false;

void setup()
{
    delay(1000);
    Serial.begin(9600);
    bool tmp[3] = {0, 0, 0};
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    light1.init(0, millis(), TrafficLight::TrafficLight(5, 6, 7, tmp));
    while (!Serial)
        ;
    // light2.init()
}

void loop()
{
    // if (light1.update())
    //     Serial.println(light1.getCurrentState());
    // light2.update();
}
