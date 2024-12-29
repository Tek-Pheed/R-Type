/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#include <any>
#include "EngineLevelManager.hpp"
#include "GameEvents.hpp"
#if defined(WIN32)
    #define NOMINMAX
#endif

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineECSManager.hpp"
#include "Entity.hpp"
#include "GameSystems.hpp"

#include "game.hpp"

// Networking &game::getNetworking()
// {
//     return (_refNetwork);
// }

void Game::LoadTexture()
{
    static const char *files[] = {"assets/sprites/r-typesheet42.gif",
        "assets/sprites/r-typesheet31.gif", "assets/sprites/r-typesheet1.gif",
        "assets/background/background.png"};
    static const char *names[] = {
        "playerTexture", "enemyTexture", "bulletTexture", "backgroundTexture"};

    for (size_t i = 0; i < sizeof(files) / sizeof(files[0]); i++) {
        assetManager.loadAsset(
            files[i], names[i], &sf::Texture::loadFromFile, sf::IntRect());
    }
    assetManager.getAsset<sf::Texture>("backgroundTexture").setRepeated(true);
    assetManager.getAsset<sf::Texture>("backgroundTexture").setRepeated(true);
    _backgroundSprite.setTextureRect(sf::Rect(0, 0, 1280, 720));
    _backgroundSprite.setTexture(
        assetManager.getAsset<sf::Texture>("backgroundTexture"));
}

void Game::gameUpdate(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    // System updates will be called automatically by the game engine.
    (void) core;
    float deltaTime_sec = std::any_cast<float>(arg);

    std::cout << "Triggered game event " << event
              << ", engine delta: " << std::to_string(deltaTime_sec)
              << std::endl;
    static int i = 0;
    i++;
    core.triggerEvent(GameEvents::ExempleEvent, i);
    return;
}

Game::Game(Engine::Core &engineRef)
    : refGameEngine(engineRef),
      entityManager(engineRef.getFeature<Engine::Feature::LevelManager<Game>>()),
      assetManager(engineRef.getFeature<Engine::Feature::AssetManager>())
{
}

Game::~Game()
{
    refGameEngine.stop();
}

bool Game::getServerMode()
{
    return _isServer;
}

void Game::setServerMode(bool mode)
{
    _isServer = mode;
}

ecs::Entity &Game::getLocalPlayer()
{
    return (entityManager.getCurrentLevel().getEntityById(_PlayerId));
}

size_t Game::getPlayerId()
{
    return (_PlayerId);
}

int Game::manageBuffers()
{
    return 0;
}

bool Game::isServer() const
{
    return (_isServer);
}

void Game::updateLocalPlayerPosition()
{
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

void Game::gameLoop(float deltaTime)
{
    auto &posSystem =
        entityManager.getCurrentLevel().getSubsystem<GameSystems::PositionSystem>();
    auto &renderSystem =
        entityManager.getCurrentLevel().getSubsystem<GameSystems::RenderSystem>();
    auto &bulletSystem =
        entityManager.getCurrentLevel().getSubsystem<GameSystems::BulletSystem>();

    (void) posSystem;
    (void) renderSystem;
    (void) bulletSystem;
    (void) deltaTime;

    if (!_isServer) {
        _window->clear();
        playEvent();
        _window->draw(_backgroundSprite);
        // clientLoop();
    }
    /*    .this->_window.clear();
    playEvent(game, game.getEntities());
    this->_window.draw(background_s);
    positionSystem.update(
        game.getEntities(), &this->_window, deltaTime, false);
    game.updateLocalplayerPosition();
    renderSystem.update(game.getEntities(), &this->_window, deltaTime, false);
    game.manageBuffers();
    bulletSystem.update(game.getEntities(),
    &this->_window, deltaTime, false); this->_window.display();
    backgroundAnimation(&background_s, &clockAnim);*/
}

std::vector<ecs::Entity> &Game::getEntities()
{
    return (entityManager.getCurrentLevel().getEntitiesVect());
}

void Game::playEvent()
{
    sf::Event event;
    std::stringstream ss;
    auto &player = entityManager.getCurrentLevel().getEntityById(_PlayerId);
    auto velocity = player.getComponent<ecs::VelocityComponent>();

    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window->close();
        }
        if (event.type == sf::Event::KeyPressed) {
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
        if (event.type == sf::Event::KeyReleased) {
            this->playerAnimations(player, "none");
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
