#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <map>
#include <functional>

class StateManager
{
private:
    JsonDocument stateDoc;

    struct VarMeta
    {
        bool isWritable;
        std::function<void(JsonVariant)> onChange;
        std::function<void()> syncToJson;
        std::function<void(JsonVariant)> syncToLocal;
    };

    std::map<String, VarMeta> registry;

    AsyncWebSocket *_ws = nullptr;
    unsigned long _updateInterval = 2000;
    unsigned long _lastUpdate = 0;
    std::function<void()> _onUpdateRoutine = nullptr;

public:
    template <typename T>
    void bindVar(const String &name, T *varPtr, bool writable, std::function<void(JsonVariant)> callback = nullptr)
    {
        stateDoc[name] = *varPtr;
        registry[name] = {
            writable, callback,
            [this, name, varPtr]()
            { this->stateDoc[name] = *varPtr; },
            [varPtr](JsonVariant val)
            { *varPtr = val.as<T>(); }};
    }

    String getJsonString()
    {
        for (auto &kv : registry)
            if (kv.second.syncToJson)
                kv.second.syncToJson();

        String output;
        serializeJson(stateDoc, output);
        return output;
    }

    bool updateFromWeb(const String &target, JsonVariant newValue)
    {
        auto it = registry.find(target);
        if (it != registry.end() && it->second.isWritable)
        {
            stateDoc[target] = newValue;
            if (it->second.syncToLocal)
                it->second.syncToLocal(newValue);
            if (it->second.onChange)
                it->second.onChange(newValue);
            return true;
        }
        return false;
    }

    void begin(AsyncWebSocket *ws, unsigned long intervalMillis = 2000)
    {
        _ws = ws;
        _updateInterval = intervalMillis;
    }

    void setUpdateRoutine(std::function<void()> routine)
    {
        _onUpdateRoutine = routine;
    }

    void loop()
    {
        if (!_ws)
            return;

        _ws->cleanupClients();

        unsigned long currentMillis = millis();

        if (currentMillis - _lastUpdate >= _updateInterval)
        {

            if (_onUpdateRoutine)
                _onUpdateRoutine();

            _ws->textAll(getJsonString());

            _lastUpdate = currentMillis;
        }
    }
};

#endif