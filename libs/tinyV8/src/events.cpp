/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** events
*/

#include "EngineEvents.hpp"

using namespace Engine::Feature;

EventManager::EventManager(Core &engineRef) : AEngineFeature(engineRef)
{
}

EventManager::~EventManager()
{
}

void EventManager::onStart(void)
{
    if (_onStartFunc)
        _onStartFunc();
}

void EventManager::onTick(float deltaTimeSec)
{
    if (_onTickFunc)
        _onTickFunc(deltaTimeSec);
}

void EventManager::onStop(void)
{
    if (_onStopFunc)
        _onStopFunc();
}
