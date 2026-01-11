#include "DistanceSensor.h"

const float SPEED_OF_SOUND = 0.0343; // cm/microsecond
const float IN_RANGE_DIS = 4.0;
const float EPSILON = 1e-6;

static int trig, echo, prev_dis = -1;

namespace DisSensor
{
void setupDistance(int trigPin, int echoPin)
{
    trig = trigPin;
    echo = echoPin;
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}

/**
 * @brief Get the distance (cm) recorded by the sensor
 * 
 * @return long 
 */
long getDistance()
{
    digitalWrite(trig, LOW);
    delayMicroseconds(5);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH);

    return duration * SPEED_OF_SOUND / 2;
}

/**
 * @brief Check if the car is in range
 * 
 * @return true 
 * @return false 
 */
bool detectCar()
{
    // Note: the range is hard-coded.
    return getDistance() <= IN_RANGE_DIS + EPSILON;
}

}