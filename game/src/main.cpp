/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <iostream>
#include <memory>

#include "EngineECSManager.hpp"
#include "Engine.hpp"
#include "EngineEvents.hpp"
#include "Networking.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "Systems.hpp"
#include "game.hpp"

void Game::LoadTexture()
{
    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture bulletTexture;
    sf::Texture backgroundTexture;

    if (playerTexture.loadFromFile("assets/sprites/r-typesheet42.gif"))
        _playerTexture = playerTexture;
    if (enemyTexture.loadFromFile("assets/sprites/r-typesheet31.gif"))
        _enemyTexture = enemyTexture;
    if (bulletTexture.loadFromFile("assets/sprites/r-typesheet1.gif"))
        _bulletTexture = bulletTexture;
    if (backgroundTexture.loadFromFile("assets/background/background.png"))
        _backgroundTexture = backgroundTexture;
    _backgroundTexture.setRepeated(true);
    _backgroundSprite.setTextureRect(sf::Rect(0, 0, 1280, 720));
    _backgroundSprite.setTexture(_backgroundTexture);
}

void Game::setupClient()
{
    _isServer = false;
    _window = std::make_unique<sf::RenderWindow>();
    sf::VideoMode videoMode(
        1280, 720, sf::VideoMode::getDesktopMode().bitsPerPixel);
    _window->create(videoMode, "R-Type");
    _window->setFramerateLimit(_refGameEngine.getTickRate());
    if (!_window->isOpen()) {
        throw std::runtime_error("Failed to create the SFML window.");
    }
}

void Game::setupServer()
{
    _isServer = true;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    Engine::Core gameEngine;
    Game game(gameEngine);

#if defined(RTYPE_SERVER)
    game.setupServer();
    std::cout << "Server mode" << std::endl;
#endif
#ifndef RTYPE_SERVER
    game.setupClient();
    std::cout << "Client mode" << std::endl;
#endif

    gameEngine.setTickRate(60);
    gameEngine.loadFeature<Engine::Feature::ECSManager>();
    gameEngine.loadFeature<Engine::Feature::EventManager>();
    // gameEngine.loadFeature<Engine::Feature::Networking>();

    // auto &manager = gameEngine.getFeature<Engine::Feature::ECSManager>();

    /*manager.createSubsxxystem<ecs::BulletSystem>();

    auto &bulletSub = manager.getSubsystem<ecs::BulletSystem>();
    auto entity = manager.createEntity();
    // entity->addComponent((bulletSub));*/

    // gameEngine.mainLoop();
}
