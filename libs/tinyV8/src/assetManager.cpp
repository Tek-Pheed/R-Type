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

void AssetManager::engineOnStart()
{
    return;
}

void AssetManager::engineOnTick(float deltaTimeSec)
{
    (void) deltaTimeSec;
    return;
}

void AssetManager::engineOnStop(void)
{
    return;
}
