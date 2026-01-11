#include "TrafficControl.h"

const int NUM_STATE = 5;
TaskHandle_t south_task,west_task;

template<int inum>
void sendUpdate()
{
    TrafficLight<inum>::dispatch(Next());
}

void southTLTask(void *parameter)
{
    static constexpr int duration[NUM_STATE] = {7000,3000,10000,5000,3000};
    int curState = 0;
    TrafficLight<Direction::SOUTH>::start();
    while (true)
    {
        vTaskDelay(duration[curState] / portTICK_PERIOD_MS);
        curState = (curState + 1) % NUM_STATE;
        sendUpdate<Direction::SOUTH>();
    }
}

void westTLTask(void *parameter)
{
    static constexpr int duration[NUM_STATE] = {7000,3000,10000,5000,3000};
    int curState = 3;
    TrafficLight<Direction::WEST>::start();
    while (true)
    {
        vTaskDelay(duration[curState] / portTICK_PERIOD_MS);
        curState = (curState + 1) % NUM_STATE;
        sendUpdate<Direction::WEST>();
    }
}

void startTL()
{
    xTaskCreatePinnedToCore(
        southTLTask,        // Function
        "SouthTLTask",      // Name
        5000,               // Stack size
        NULL,               // Parameter
        1,                  // Priority
        &south_task,        // Task handler
        1                   // Core ID
    );
    xTaskCreatePinnedToCore(
        westTLTask,         // Function
        "westTLTask",       // Name
        5000,               // Stack size
        NULL,               // Parameter
        1,                  // Priority
        &west_task,         // Task handler
        1                   // Core ID
    );
}