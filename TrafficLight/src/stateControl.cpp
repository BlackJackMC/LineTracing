#include "stateControl.h"

State::State() {}
State::State(state_name stateId, int duration, bool activePins[3]) : stateId(stateId), duration(duration)
{
    for (int i = 0; i < 3; i++)
    {
        this->activePins[i] = activePins[i];
    }
}

void StateControl::init(int startId, int curTime, TrafficLight::TrafficLight light)
{
    prevTime = curTime;
    currentState = startId;
    trafficLight = light;
}

void StateControl::update()
{
    int curTime = millis();
    if (curTime - prevTime >= stateList[currentState].duration)
    {
        currentState = (currentState + 1) % 6;
        for (int i = 0; i < 3; i++)
        {
            trafficLight.setState(i, stateList[currentState].activePins[i]);
        }
    }
}

state_name StateControl::getCurrentState()
{
    return stateList[currentState].stateId;
}