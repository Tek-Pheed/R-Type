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

using namespace Engine;

Core::Core()
{
}

Core::~Core()
{
}

void Core::onStart(void)
{
    return;
}

void Core::onTick(float deltaTimeSec)
{
    for (const auto &sys : _features) {
        sys.second->onTick(deltaTimeSec);
    }
}

void Core::onStop(void)
{
    return;
}

void Core::mainLoop(void)
{
    auto now = std::chrono::time_point_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now());
    auto last = std::chrono::time_point_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now());

    this->onStart();
    while (_running) {
        now = std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now());
        auto next = std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now());
        next += std::chrono::microseconds(1000000 / _tickRate);
        auto deltaTimeUs =
            std::chrono::duration_cast<std::chrono::microseconds>(now - last)
                .count();
        this->onTick((float) deltaTimeUs / 1000000.0f);
        last = now;
        std::this_thread::sleep_until(next);
    }
    this->onStop();
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
