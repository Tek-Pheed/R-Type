/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <any>
#include <iostream>
#include <memory>
#include <string>

#include "Components.hpp"
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "GameEvents.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
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

void handleCustomEvent(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    int i = std::any_cast<int>(arg);

    std::cout << "Triggered custom event " << event << " with data=" << i
              << ", engine delta: " << std::to_string(core.getDeltaTime_Sec())
              << std::endl;
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
    gameEngine.loadFeature<Engine::Feature::LevelManager<Game>>();
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

    gameEngine.addEventBinding(GameEvents::ExempleEvent, handleCustomEvent);
    gameEngine.addEventBinding<Game>(
        Engine::Events::OnTick, &Game::gameUpdate, game);

    /*manager.createSubsxxystem<ecs::BulletSystem>();

    auto &bulletSub = manager.getSubsystem<ecs::BulletSystem>();
    auto entity = manager.createEntity();
    // entity->addComponent((bulletSub));*/

    auto &levelMan =
        gameEngine.getFeature<Engine::Feature::LevelManager<Game>>();

    (void) levelMan;

    levelMan.createNewLevel("levelTest");
    levelMan.createNewLevel("finalLevel");

    levelMan.switchLevel("levelTest", false);

    auto &ent = levelMan.getCurrentLevel().createEntity();
    ent.addComponent(std::make_shared<ecs::PositionComponent>());

    auto &enta = levelMan.getSpecificLevel("finalLevel").createEntity();
    // enta.addComponent(std::make_shared<ecs::NameComponent>("toto"));
    enta.addComponent(std::make_shared<ecs::PositionComponent>(50, 50));
    enta.addComponent(std::make_shared<ecs::BulletComponent>(false));
    enta.addComponent(
        std::make_shared<ecs::RenderComponent>(ecs::RenderComponent::SPRITE));
    auto texture = sf::Texture();
    texture.loadFromFile("assets/sprites/r-typesheet1.gif");
    auto sprite = sf::Sprite(texture);
    enta.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 100, 100));

    // enta.addComponent(std::make_shared<ecs::VelocityComponent>(1,1));
    levelMan.getSpecificLevel("finalLevel")
        .createSubsystem<GameSystems::PositionSystem>()
        .initSystem(game);
    levelMan.getSpecificLevel("finalLevel")
        .createSubsystem<GameSystems::BulletSystem>()
        .initSystem(game);
    levelMan.getSpecificLevel("finalLevel")
        .createSubsystem<GameSystems::RenderSystem>()
        .initSystem(game);

    levelMan.switchLevel("finalLevel");

    // gameEngine.triggerEvent(GameEvents::ExempleEvent, 36);

    gameEngine.mainLoop();
}
