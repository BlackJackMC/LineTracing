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
        Serial.println("South: Running");
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
        Serial.println("West: Running");
        vTaskDelay(duration[curState] / portTICK_PERIOD_MS);
        curState = (curState + 1) % NUM_STATE;
        sendUpdate<Direction::WEST>();
    }
}

void setTLPinMode(int red,int yellow,int green)
{
    pinMode(red,OUTPUT);
    pinMode(yellow,OUTPUT);
    pinMode(green,OUTPUT);
}

void startTL()
{
    setTLPinMode(
        static_cast<int>(Pin_South::RED_PIN),
        static_cast<int>(Pin_South::YELLOW_PIN),
        static_cast<int>(Pin_South::GREEN_PIN)
    );
    setTLPinMode(
        static_cast<int>(Pin_West::RED_PIN),
        static_cast<int>(Pin_West::YELLOW_PIN),
        static_cast<int>(Pin_West::GREEN_PIN)
    );

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