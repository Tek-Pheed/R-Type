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
#include <sstream>
#include <vector>
#include "Components.hpp"
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "GameAssets.hpp"
#include "GameEvents.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

using namespace RType;

size_t getNewId()
{
    static size_t id = 0;

    id++;
    return (id);
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

void GameInstance::loadAssets()
{
    try {
        for (const auto asset : Asset::getAllAssetsOfType<sf::Texture>()) {
            refAssetManager.loadAsset(asset->path, asset->identifier,
                &sf::Texture::loadFromFile, sf::IntRect());
        }
        for (const auto asset : Asset::getAllAssetsOfType<sf::Font>()) {
            refAssetManager.loadAsset(
                asset->path, asset->identifier, &sf::Font::loadFromFile);
        }
    } catch (const std::exception &e) {
        std::cout << "Failed to an load asset with error: " << e.what()
                  << std::endl;
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
        manageBuffers();
        for (auto &pl : getAllPlayers()) {
            playerAnimations(pl.get());
        }
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

int RType::GameInstance::manageBuffers()
{
    if (!isServer() && !_isConnectedToServer)
        return (-1);
    auto packets = refNetworkManager.readAllPackets();
    if (packets.size() == 0)
        return 0;

    for (auto &buff : packets) {
        std::string buffer = buff;
        std::string codeStr = buffer.substr(0, 3);
        int code = atoi(codeStr.c_str());
        int code_int = is_code_valid(code);
        std::vector<std::string> tokens;
        if (code_int == -1) {
            return -1;
        }
        std::string str = buffer.substr(4, buffer.size() - 4);
        std::istringstream ss(str);
        std::string token;
        std::cout << "Managing Buffer: " << buffer << std::endl;
        while (std::getline(ss, token, ' ')) {
            tokens.push_back(token);
        }
        switch (code_int) {
            case 0: handleNetworkPlayers(code, tokens); break;
            // case 1: handle_enemy(code, tokens); break;
            // case 2: handle_terrain(code, tokens); break;
            // case 3: handle_mechs(code, tokens); break;
            case 9:
                if (isServer()) {
                    serverHanlderValidateConnection(code, tokens);
                } else {
                    clientHandlerConnection(code, tokens);
                }
                break;
            default: break;
        }
        ss.clear();
    }
    return 0;
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
