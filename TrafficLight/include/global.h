#pragma once

inline QueueHandle_t msgQueue;

struct Data {
    char type;
    bool isSouth;
    int value;
};