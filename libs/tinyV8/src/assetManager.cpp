/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AssetManager
*/

#include "EngineAssetManager.hpp"

using namespace Engine::Feature;

AssetManager::AssetManager(Core &engineRef) : AEngineFeature(engineRef)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::onStart()
{
    return;
}

void AssetManager::onTick(float deltaTimeSec)
{
    (void) deltaTimeSec;
    return;
}

void AssetManager::onStop(void)
{
    return;
}
