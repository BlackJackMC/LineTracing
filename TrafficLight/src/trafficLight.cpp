#include "TrafficLight.h"

namespace TrafficLight
{

    TrafficLight::TrafficLight(int redPin, int yellowPin, int greenPin, bool state[3])
    {
        this->pin[RED] = redPin;
        this->pin[YELLOW] = yellowPin;
        this->pin[GREEN] = greenPin;

        this->state[RED] = state[RED];
        this->state[YELLOW] = state[YELLOW];
        this->state[GREEN] = state[GREEN];
    }

    void TrafficLight::toggleLight(Light light)
    {
        state[light] = !state[light];
        digitalWrite(pin[light], state[light]);
    }

    bool TrafficLight::getState(Light light) const
    {
        return state[light];
    }

    void TrafficLight::setState(Light light, bool newState)
    {
        state[light] = newState;
        digitalWrite(pin[light], state[light]);
    }
}