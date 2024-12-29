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
#include <memory>
#include <sstream>
#include <vector>
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "GameAssets.hpp"
#include "GameEvents.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/Texture.hpp"

using namespace RType;

const std::vector<const Asset::AssetStore *> getAllAsset()
{
    std::vector<const Asset::AssetStore *> vect;

    for (size_t i = 0; i < sizeof(Asset::assets) / sizeof(Asset::assets[0]);
        i++) {
        vect.emplace_back(&Asset::assets[i]);
    }
    return (vect);
}

void handleCustomEvent(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    auto i = std::any_cast<std::string>(arg);

    std::cout << "Triggered custom event " << event << " with data=" << i
              << ", engine delta: " << std::to_string(core.getDeltaTime_Sec())
              << std::endl;
}

void GameInstance::loadTexture()
{
    for (const auto asset : Asset::getAllAssetsOfType<sf::Texture>()) {
        try {
            assetManager.loadAsset(asset->path, asset->identifier,
                &sf::Texture::loadFromFile, sf::IntRect());
        } catch (const std::exception &e) {
            std::cout << "Failed to load asset " << asset->identifier
                      << " with error: " << e.what() << std::endl;
        }
    }
}

void GameInstance::gameUpdate(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    // System updates are called automatically by the game engine.
    (void) core;
    float deltaTime_sec = std::any_cast<float>(arg);

    std::cout << "Triggered game update " << event
              << ", engine delta: " << std::to_string(deltaTime_sec)
              << std::endl;
    static int i = 0;
    i++;
    core.triggerEvent(GameEvents::EVENT_ExempleEvent, i);
    if (!_isServer) {
        playEvent();
        updateLocalPlayerPosition();
        // backgroundAnimation(&background_s, &clockAnim);
    }
    manageBuffers();
}

GameInstance::GameInstance(Engine::Core &engineRef)
    : refGameEngine(engineRef),
      entityManager(
          engineRef.getFeature<Engine::Feature::LevelManager<GameInstance>>()),
      assetManager(engineRef.getFeature<Engine::Feature::AssetManager>()),
      networkManager(
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

ecs::Entity &GameInstance::getLocalPlayer()
{
    if (!hasPlayer())
        throw ErrorClass("No player was attached to the client");
    return (entityManager.getCurrentLevel().getEntityById((size_t) _PlayerId));
}

int GameInstance::getPlayerId() const
{
    return (_PlayerId);
}

void GameInstance::setPlayerId(int id)
{
    _PlayerId = id;
}

int GameInstance::manageBuffers()
{
    return 0;
}

bool GameInstance::isServer() const
{
    return (_isServer);
}

bool GameInstance::hasPlayer(void) const
{
    if (_PlayerId == -1) {
        return (false);
    }
    return (true);
}

void GameInstance::updateLocalPlayerPosition()
{
    if (!hasPlayer())
        return;
    auto position = getLocalPlayer().getComponent<ecs::PositionComponent>();
    if (position) {
        float oldX = position->getOldX();
        float oldY = position->getOldY();
        float x = position->getX();
        float y = position->getY();

        if (oldX != x || oldY != y) {
            std::stringstream ss;
            ss << "102 " << _PlayerId << " " << x << " " << y << "\t\n";
            // writeToServer(ss.str(), System::Network::ISocket::UDP);
        }
    }
}

std::vector<ecs::Entity> &RType::GameInstance::getEntities()
{
    return (entityManager.getCurrentLevel().getEntitiesVect());
}

void GameInstance::playEvent()
{
    sf::Event event;
    std::stringstream ss;

    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window->close();
            refGameEngine.stop();
        }
        if (hasPlayer() && event.type == sf::Event::KeyPressed) {
            auto &player = getLocalPlayer();
            auto velocity = player.getComponent<ecs::VelocityComponent>();
            if (event.key.code == sf::Keyboard::Up) {
                velocity->setVy(-200.0f);
                this->playerAnimations(player, "top");
            } else if (event.key.code == sf::Keyboard::Down) {
                velocity->setVy(200.0f);
                this->playerAnimations(player, "down");
            } else if (event.key.code == sf::Keyboard::Right) {
                velocity->setVx(200.0f);
                this->playerAnimations(player, "right");
            } else if (event.key.code == sf::Keyboard::Left) {
                velocity->setVx(-200.0f);
                this->playerAnimations(player, "left");
            } else if (event.key.code == sf::Keyboard::Space) {
                playerShoot(player);
            }
        }
        if (hasPlayer() && event.type == sf::Event::KeyReleased) {
            auto &player = getLocalPlayer();
            auto velocity = player.getComponent<ecs::VelocityComponent>();
            this->playerAnimations(getLocalPlayer(), "none");
            if (event.key.code == sf::Keyboard::Up
                || event.key.code == sf::Keyboard::Down) {
                velocity->setVy(0.0f);
            }
            if (event.key.code == sf::Keyboard::Left
                || event.key.code == sf::Keyboard::Right) {
                velocity->setVx(0.0f);
            }
        }
    }
}
