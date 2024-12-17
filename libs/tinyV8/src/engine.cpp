/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** engine
*/

#include "Engine.hpp"
#include <memory>

using namespace Engine;

Core::Core()
{
}

Core::~Core()
{
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

Core::UpdateMode Core::getUpdateMode(void)
{
    return (_updateMode);
}

uint16_t Core::getTickRate(void)
{
    return (_tickRate);
}

void Core::setTickRate(uint16_t tickRateHZ)
{
    _tickRate = tickRateHZ;
}
