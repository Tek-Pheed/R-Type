/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Game.hpp"
#include <any>
#include <exception>
#include <iostream>
#include <vector>
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "GameAssets.hpp"
#include "GameEvents.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/Texture.hpp"

using namespace RType;

int GameInstance::is_code_valid(int code)
{
    if (code >= P_CONN && code <= P_DISCONN)
        return 0;
    if (code >= E_SPAWN && code <= E_DMG)
        return 1;
    if (code >= T_SPAWN && code <= T_DEAD)
        return 2;
    if (code >= M_WAVE && code <= M_GOVER)
        return 3;
    if (code >= C_INIT_UDP && code <= C_AUTH)
        return 9;
    return -1;
}

const std::vector<const Asset::AssetStore *> getAllAsset()
{
    std::vector<const Asset::AssetStore *> vect;

    for (size_t i = 0; i < sizeof(Asset::assets) / sizeof(Asset::assets[0]);
        i++) {
        vect.emplace_back(&Asset::assets[i]);
    }
    return (vect);
}

void GameInstance::loadTexture()
{
    for (const auto asset : Asset::getAllAssetsOfType<sf::Texture>()) {
        try {
            refAssetManager.loadAsset(asset->path, asset->identifier,
                &sf::Texture::loadFromFile, sf::IntRect());
        } catch (const std::exception &e) {
            std::cout << "Failed to load asset " << asset->identifier
                      << " with error: " << e.what() << std::endl;
        }
    }
}

void GameInstance::gamePreTick(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) event;
    (void) core;
    (void) arg;
    if (!isServer()) {
        getWindow().clear();
    }
}

void GameInstance::gameTick(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    // System updates are called automatically by the game engine.
    (void) core;
    (void) event;
    float deltaTime_sec = std::any_cast<float>(arg);
    (void) deltaTime_sec;

    if (!_isServer) {
        playEvent();
        updateLocalPlayerPosition();
        clientManageBuffers();
    }
}

void GameInstance::gamePostTick(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) event;
    (void) core;
    (void) arg;
    if (!isServer()) {
        getWindow().display();
    }
}

GameInstance::GameInstance(Engine::Core &engineRef)
    : refGameEngine(engineRef),
      refEntityManager(
          engineRef.getFeature<Engine::Feature::LevelManager<GameInstance>>()),
      refAssetManager(engineRef.getFeature<Engine::Feature::AssetManager>()),
      refNetworkManager(
          engineRef.getFeature<Engine::Feature::NetworkingManager>())
{
}

GameInstance::~GameInstance()
{
    refGameEngine.stop();
}

bool GameInstance::getServerMode()
{
    return _isServer;
}

bool GameInstance::isServer() const
{
    return (_isServer);
}

std::vector<ecs::Entity> &RType::GameInstance::getEntities()
{
    return (refEntityManager.getCurrentLevel().getEntitiesVect());
}
