/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <iostream>
#include <memory>

#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineECSManager.hpp"
#include "EngineEvents.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "game.hpp"


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
    LoadTexture();
}

void Game::setupServer()
{
    _isServer = true;
}

static int print_help()
{
    std::cout << "USAGE: ./game {IP} {PORT TCP} {PORT UDP}" << std::endl;
    return 0;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    if (argc == 1) {
        print_help();
        return (0);
    }

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


    /*manager.createSubsxxystem<ecs::BulletSystem>();

    auto &bulletSub = manager.getSubsystem<ecs::BulletSystem>();
    auto entity = manager.createEntity();
    // entity->addComponent((bulletSub));*/

    gameEngine.mainLoop();
}
