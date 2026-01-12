#pragma once
#include <Arduino.h>
#include "tinyfsm.hpp"

const int BLINK_STEP_DURATION = 500;

enum Light
{
    RED,
    YELLOW,
    GREEN,
};

enum Direction
{
    SOUTH,
    WEST
};

enum class Pin_South
{
    RED_PIN = 12,
    YELLOW_PIN = 14,
    GREEN_PIN = 27,
};

enum class Pin_West
{
    RED_PIN = 26,
    YELLOW_PIN = 25,
    GREEN_PIN = 33,
};

// Events

struct Next : tinyfsm::Event{};

// Traffic Light FSM
template<int inum>
class TrafficLight : public tinyfsm::Fsm< TrafficLight<inum> >
{
public:
    inline static constexpr int pin[3] = { // Pls How do I rework this
        inum == Direction::SOUTH ? static_cast<int>(Pin_South::RED_PIN) : static_cast<int>(Pin_West::RED_PIN),
        inum == Direction::SOUTH ? static_cast<int>(Pin_South::YELLOW_PIN) : static_cast<int>(Pin_West::YELLOW_PIN),
        inum == Direction::SOUTH ? static_cast<int>(Pin_South::GREEN_PIN) :static_cast<int>(Pin_West::GREEN_PIN)
    }; // R - G - Y

    // Helper functions

    void resetOutput();

    // FSM functions

    void react(tinyfsm::Event const &) {};

    virtual void react(Next const &) {};

    virtual void entry(void) {};
    virtual void exit(void) {};
};

// States
template<int inum>
class Red : public TrafficLight<inum>
{
    void react(Next const &) override;
    void entry(void) override;
};

template<int inum>
class RedYellow : public TrafficLight<inum>
{
    void react(Next const &) override;
    void entry(void) override;
};

template<int inum>
class Green : public TrafficLight<inum>
{
    void react(Next const &) override;
    void entry(void) override;
};

template<int inum>
class GreenBlink : public TrafficLight<inum>
{
    void react(Next const &) override;
    void entry(void) override;
    void exit(void) override;

    inline static TaskHandle_t blinkTaskHandle = nullptr;
};

template<int inum>
class Yellow : public TrafficLight<inum>
{
    void react(Next const &) override;
    void entry(void) override;
};
