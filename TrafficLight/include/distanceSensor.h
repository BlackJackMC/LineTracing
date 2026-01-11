#pragma once
#include <Arduino.h>

namespace DisSensor 
{

    void setupDistance(int trigPin, int echoPin);

    long getDistance();

    bool detectCar();

}