#pragma once
#include "TrafficLight.h"

extern TaskHandle_t south_task,west_task;

void sendUpdate(int inum);

void startTL(void);