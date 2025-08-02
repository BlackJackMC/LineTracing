#pragma once
#include <Arduino.h>

namespace TrafficLight
{
    enum Light
    {
        RED,
        GREEN,
        YELLOW,
    };
    class TrafficLight
    {
    private:
        int pin[3];
        bool state[3];

    public:
        TrafficLight() {};
        TrafficLight(int redPin, int greenPin, int yellowPin, bool state[3]);
        void toggleLight(Light light);

        bool getState(Light light) const;

        void setState(Light light, bool new_state);
    };
}