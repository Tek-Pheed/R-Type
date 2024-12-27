/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <iostream>
#include <memory>
#include <utility>

#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineECSManager.hpp"
#include "EngineEvents.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "Systems.hpp"
#include "game.hpp"

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

void Game::setupClient()
{
    _isServer = false;
    _window = std::make_unique<sf::RenderWindow>();
    sf::VideoMode videoMode(
        1280, 720, sf::VideoMode::getDesktopMode().bitsPerPixel);
    _window->create(videoMode, "R-Type");
    _window->setFramerateLimit(refGameEngine.getTickRate());
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
    gameEngine.setTickRate(60);
    gameEngine.loadFeature<Engine::Feature::ECSManager<Game>>();
    gameEngine.loadFeature<Engine::Feature::EventManager>();
    gameEngine.loadFeature<Engine::Feature::AssetManager>();
    // gameEngine.loadFeature<Engine::Feature::Networking>();

    Game game(gameEngine);

#if defined(RTYPE_SERVER)
    game.setupServer();
    std::cout << "Server mode" << std::endl;
#endif
#ifndef RTYPE_SERVER
    game.setupClient();
    std::cout << "Client mode" << std::endl;
#endif

    // auto &manager = gameEngine.getFeature<Engine::Feature::ECSManager>();

    /*manager.createSubsxxystem<ecs::BulletSystem>();

    auto &bulletSub = manager.getSubsystem<ecs::BulletSystem>();
    auto entity = manager.createEntity();
    // entity->addComponent((bulletSub));*/

    // gameEngine.mainLoop();
}
