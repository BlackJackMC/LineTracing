#pragma once
#include "trafficLight.h"
#include <Arduino.h>
#include <vector>

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

class StateControl
{
private:
    int currentState;
    int prevTime;
    TrafficLight::TrafficLight trafficLight;
    State stateList[6] = {

    };

public:
    void init(int, int, TrafficLight::TrafficLight);
    void update();
    state_name getCurrentState();
};