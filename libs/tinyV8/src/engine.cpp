/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** engine
*/

#include "Engine.hpp"
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

using namespace Engine;

Core::Core()
{
}

Core::~Core()
{
}

void Core::engineOnStart(void)
{
    triggerEvent(Engine::Events::OnStart);
}

void Core::engineOnTick(float deltaTimeSec)
{
    for (const auto &sys : _features) {
        sys.second->engineOnTick(deltaTimeSec);
    }
}

void Core::engineOnStop(void)
{
    triggerEvent(Engine::Events::OnStop);
}

void Core::mainLoop(void)
{
    auto now = std::chrono::time_point_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now());
    auto last = std::chrono::time_point_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now());

    this->engineOnStart();
    while (_running) {
        now = std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now());
        auto next = std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now());
        next += std::chrono::microseconds(1000000 / _tickRate);
        auto deltaTimeUs =
            std::chrono::duration_cast<std::chrono::microseconds>(now - last)
                .count();
        _currentDeltaTime = (float) deltaTimeUs / 1000000.0f;
        this->engineOnTick(_currentDeltaTime);
        triggerEvent(Engine::Events::OnTick, _currentDeltaTime);
        last = now;
        if (_updateMode == LOCKED)
            std::this_thread::sleep_until(next);
    }
    this->engineOnStop();
}

void Core::start(void)
{
    _running = true;
}

void Core::stop(void)
{
    _running = false;
}

void Core::setUpdateMode(UpdateMode mode)
{
    _updateMode = mode;
}

Core::UpdateMode Core::getUpdateMode(void) const
{
    return (_updateMode);
}

uint16_t Core::getTickRate(void) const
{
    return (_tickRate);
}

void Core::setTickRate(uint16_t tickRateHZ)
{
    _tickRate = tickRateHZ;
}

float Core::getDeltaTime_Sec(void)
{
    return (_currentDeltaTime);
}

void Core::addEventBinding(
    const Events::EventType &event, Events::EventHanlderType eventHandler)
{
    if (!_events.contains(event)) {
        _events.emplace(event, std::vector<Events::EventHanlderType>());
    }
    _events[event].emplace_back(eventHandler);
}

bool Core::clearEventBinding(const Events::EventType &event)
{
    if (!_events.contains(event)) {
        return (false);
    }
    _events[event].clear();
    return (true);
}

bool Core::triggerEvent(const Events::EventType &event, std::any extraArg)
{
    if (!_events.contains(event)) {
        return (false);
    }
    for (auto f : _events[event]) {
        f(event, *this, extraArg);
    }
    return (true);
}
