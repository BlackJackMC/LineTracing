#include "TrafficControl.h"

namespace TrafficControl
{

    State::State(state_name stateId, int duration, bool activePins[3]) : stateId(stateId), duration(duration)
    {
        for (int i = 0; i < 3; i++)
        {
            this->activePins[i] = activePins[i];
        }
    }

#define PIN(a, b, c) \
    (bool[3]) { a, b, c }
    State stateList[8] = {
        State(RED, 7000, PIN(1, 0, 0)),
        State(RED_YELLOW, 3000, PIN(1, 1, 0)),
        State(GREEN, 10000, PIN(0, 0, 1)),
        State(GREEN_BLINK1, 500, PIN(0, 0, 1)),
        State(GREEN_BLINK2, 500, PIN(0, 0, 1)),
        State(GREEN_BLINK3, 500, PIN(0, 0, 1)),
        State(GREEN_BLINK4, 500, PIN(0, 0, 1)),
        State(YELLOW, 3000, PIN(0, 1, 0)),
    };
#undef PIN

    void TaskScheduler::init(int startId, int curTime, TrafficLight::TrafficLight light)
    {
        prevTime = curTime;
        currentState = startId;
        trafficLight = light;
    }

    bool TaskScheduler::update()
    {
        int curTime = millis();
        if (curTime - prevTime >= stateList[currentState].duration)
        {
            currentState = (currentState + 1) % 6;
            static TrafficLight::Light arr[3] = {TrafficLight::Light::RED,
                                                 TrafficLight::Light::GREEN,
                                                 TrafficLight::Light::YELLOW};
            for (int i = 0; i < 3; i++)
            {
                trafficLight.setState(arr[i], 0);
            }
            for (int i = 0; i < 3; i++)
            {
                trafficLight.setState(arr[i], stateList[currentState].activePins[i]);
            }
            prevTime = curTime;
            return true;
        }
        return false;
    }

    state_name TaskScheduler::getCurrentState()
    {
        return stateList[currentState].stateId;
    }
}