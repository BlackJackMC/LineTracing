#pragma once
#include <Arduino.h>

void setupDistance(int id, int trigPin, int echoPin);

long getDistance(int id);

bool detectCar();