#include "traffic_control.h"
#include "Net.h"
#include "global.h"
#include <ArduinoJson.h>

const int NUM_STATE = 5;
TaskHandle_t south_task,west_task;

template<int inum>
void sendUpdate()
{
    TrafficLight<inum>::dispatch(Next());
}

void southTLTask(void *parameter)
{
    static constexpr int duration[NUM_STATE] = {7000,3000,10000,2000,3000};
    int curState = 0;
    Data data;
    data.type = 'l';
    data.isSouth = 1;
    TrafficLight<Direction::SOUTH>::start();
    while (true)
    {
        Serial.println("South: Running ");
        
        data.value = curState;
        xQueueSend(msgQueue, &data, portMAX_DELAY);
        
        vTaskDelay(duration[curState] / portTICK_PERIOD_MS);
        curState = (curState + 1) % NUM_STATE;
        sendUpdate<Direction::SOUTH>();
    }
}

void westTLTask(void *parameter)
{
    static constexpr int duration[NUM_STATE] = {7000,3000,10000,2000,3000};
    int curState = 2;
    Data data;
    data.type = 'l';
    data.isSouth = 0;
    TrafficLight<Direction::WEST>::start();
    while (true)
    {
        Serial.println("West: Running");

        data.value = curState;
        xQueueSend(msgQueue, &data, portMAX_DELAY);

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
        0                   // Core ID
    );
}