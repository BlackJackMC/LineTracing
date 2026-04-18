#include "distance_sensor.h"

const float SPEED_OF_SOUND = 0.0343; // cm/microsecond
const float IN_RANGE_DIS = 4.0;
const float EPSILON = 1e-6;

static int trig[2], echo[2], prev_dis = -1;

void setupDistance(int id,int trigPin, int echoPin)
{
    trig[id] = trigPin;
    echo[id] = echoPin;
    pinMode(trig[0], OUTPUT);
    pinMode(echo[0], INPUT);
}

/**
 * @brief Get the distance (cm) recorded by the sensor
 * 
 * @return long 
 */
long getDistance(int id)
{
    digitalWrite(trig[id], LOW);
    delayMicroseconds(5);
    digitalWrite(trig[id], HIGH);
    delayMicroseconds(10);
    digitalWrite(trig[id], LOW);

    long duration = pulseIn(echo[id], HIGH);

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
    return getDistance(0) <= IN_RANGE_DIS + EPSILON || getDistance(1) <= IN_RANGE_DIS + EPSILON ;
}
