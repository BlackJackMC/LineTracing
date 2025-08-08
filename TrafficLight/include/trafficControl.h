#pragma once
#include "TrafficLight.h"
#include <Arduino.h>
#include <vector>

namespace TrafficControl
{

    enum state_name
    {
        RED,
        RED_YELLOW,
        GREEN,
        GREEN_BLINK1,
        GREEN_BLINK2,
        GREEN_BLINK3,
        GREEN_BLINK4,
        YELLOW
    };

    class State
    {
    public:
        bool activePins[3]; // R - Y - G
        state_name stateId;
        int duration;
        State() {};
        State(state_name stateId, int duration, bool activePins[3]);
    };

    extern State stateList[8];

    class TaskScheduler
    {
    private:
        int currentState;
        int prevTime;
        TrafficLight::TrafficLight trafficLight;

    public:
        TaskScheduler() {};
        void init(int startId, int curTime, TrafficLight::TrafficLight light);
        bool update();
        state_name getCurrentState();
    };
}