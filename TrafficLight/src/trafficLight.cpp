#include "TrafficLight.h"

const int BLINK_DELAY = 500;

// Default entry
template<int inum>
void TrafficLight<inum>::resetOutput()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(pin[i], LOW);
    }
}

template<int inum>
void Red<inum>::react(Next const &event)
{
    using base = TrafficLight<inum>;
    base::template transit<RedYellow<inum>>();
}
template<int inum>
void Red<inum>::entry()
{
    using base = TrafficLight<inum>;
    base::resetOutput();
    digitalWrite(base::pin[RED], HIGH);
}

template<int inum>
void RedYellow<inum>::react(Next const &event)
{
    using base = TrafficLight<inum>;
    base::template transit<Green<inum>>();
}
template<int inum>
void RedYellow<inum>::entry()
{
    using base = TrafficLight<inum>;
    base::resetOutput();
    digitalWrite(base::pin[RED], HIGH);
    digitalWrite(base::pin[YELLOW], HIGH);
}

template<int inum>
void Green<inum>::react(Next const &event)
{
    using base = TrafficLight<inum>;
    base::template transit<GreenBlink<inum>>();
}
template<int inum>
void Green<inum>::entry()
{
    using base = TrafficLight<inum>;
    base::resetOutput();
    digitalWrite(base::pin[GREEN], HIGH);
}

void blinkTask(void *green_pin_raw)
{
    int green_pin_casted = *((int*)green_pin_raw);
    while (true)
    {
        digitalWrite(green_pin_casted,HIGH);
        vTaskDelay(BLINK_DELAY / portTICK_PERIOD_MS);
        digitalWrite(green_pin_casted,LOW);
        vTaskDelay(BLINK_DELAY / portTICK_PERIOD_MS);
    }
}


template<int inum>
void GreenBlink<inum>::react(Next const &event)
{
    using base = TrafficLight<inum>;
    base::template transit<Yellow<inum>>();
}
template<int inum>
void GreenBlink<inum>::entry()
{
    using base = TrafficLight<inum>;
    base::resetOutput();
    xTaskCreatePinnedToCore(
        blinkTask,          // Function
        "GreenBlinkTask",   // Name
        3000,               // Stack size
        (void*)&base::pin[GREEN],  // Parameter
        1,                  // Priority
        &blinkTaskHandle,   // Task handler
        1                   // Core ID
    );
}
template<int inum>
void GreenBlink<inum>::exit() 
{
    vTaskDelete(blinkTaskHandle);
}

template<int inum>
void Yellow<inum>::react(Next const &event)
{
    using base = TrafficLight<inum>;
    base::template transit<RedYellow<inum>>();
}
template<int inum>
void Yellow<inum>::entry()
{
    using base = TrafficLight<inum>;
    base::resetOutput();
    digitalWrite(base::pin[YELLOW], HIGH);
}

FSM_INITIAL_STATE(TrafficLight<Direction::SOUTH>,Red<Direction::SOUTH>)
FSM_INITIAL_STATE(TrafficLight<Direction::WEST>,Green<Direction::WEST>)
