#include "DistanceSensor.h"

const float SPEED_OF_SOUND = 0.0343; // cm/microsecond

static int trig, echo;

void setupDis(int trigPin, int echoPin)
{
    trig = trigPin;
    echo = echoPin;
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}

long getDis()
{
    digitalWrite(trig, LOW);
    delayMicroseconds(5);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH);

    return duration * SPEED_OF_SOUND / 2;
}