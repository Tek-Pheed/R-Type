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
#include "EngineNetworking.hpp"
#include "game.hpp"
#include "system.hpp"

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

    // Engine::Core gameEngine;
    // Game game(gameEngine);

#if defined(RTYPE_SERVER)// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** game
// */

// // #include <cstdint>
// // #include "Networking.hpp"
// // #include "Systems.hpp"
// // #if defined(WIN32)
// //     #define NOMINMAX
// // #endif

// // #include <ctime>
// // #include "RenderClass.hpp"
// // #include "game.hpp"
// // #include "server.hpp"
// // #include "system_network.hpp"

// // void game::setServerMode(bool mode)
// // {
// //     _isServer = mode;
// // }

// // bool game::isServer(void)
// // {
// //     return (_isServer);
// // }

// // int print_help()
// // {
// //     std::cout << "USAGE: ./game {IP} {PORT TCP} {PORT UDP}" << std::endl;
// //     return 0;
// // }

// // Networking &game::getNetworking()
// // {
// //     return (_refNetwork);
// // }

// // void game::gameUpdate(float deltaTime_sec)
// // {
// //     ecs::BulletSystem &bulletSystem = getSubSystem<ecs::BulletSystem>();
// //     ecs::PositionSystem &positionSystem =
// //     getSubSystem<ecs::PositionSystem>();

// //     bulletSystem.update(getEntities(), nullptr, deltaTime_sec, isServer());
// //     positionSystem.update(getEntities(), nullptr, deltaTime_sec,
// //     isServer());
// // }

// // int main(int argc, char **argv)
// // {
// //     System::Network::initNetwork();
// //     Networking net;
// //     game game(net);
// //     game.setServerMode(false);

// // #if defined(RTYPE_SERVER)
// //     game.setServerMode(true);
// // #endif
// //     if (argc != 4)
// //         return print_help();
// //     if (!atoi(argv[1]) || !atoi(argv[2]))
// //         return print_help();
// //     int portTCP = atoi(argv[2]);
// //     int portUDP = atoi(argv[3]);
// //     std::cout << "Connecting to: " << argv[1] << " on port " << portTCP << "
// //     "
// //               << portUDP << std::endl;

// //     if (game.isServer()) {
// //         net.setupServer((uint16_t) portTCP, (uint16_t) portUDP);
// //         serverLoop(net, game);
// //     } else {
// //         net.setupClient(
// //             (uint16_t) portTCP, (uint16_t) portUDP, std::string(argv[1]));
// //         RenderClass render(1280, 720, "R-Type", 60);
// //         game.setRenderClass(&render);
// //         std::cout << "game connected" << std::endl;
// //         game.createPlayer();
// //         game.getRenderClass()->renderWindow(game);
// //     }
// // }

// #include <iostream>
// #include <memory>
// #include <sstream>

// #include "EngineECSManager.hpp"
// #include "Engine.hpp"
// #include "Systems.hpp"

// #include "game.hpp"

// Game::Game(Engine::Core &engineRef) : _refGameEngine(engineRef)
// {
// }

// Game::~Game()
// {
//     _refGameEngine.stop();
// }

// bool Game::getServerMode()
// {
//     return _isServer;
// }

// void Game::setServerMode(bool mode)
// {
//     _isServer = mode;
// }

// void Game::gameLoop(float deltaTime)
// {
//     auto &manager = _refGameEngine.getFeature<Engine::Feature::ECSManager>();
//     auto &posSystem = manager.getSubsystem<ecs::PositionSystem>();
//     auto &renderSystem = manager.getSubsystem<ecs::RenderSystem>();
//     auto &bulletSystem = manager.getSubsystem<ecs::BulletSystem>();

//     (void) posSystem;
//     (void) renderSystem;
//     (void) bulletSystem;
//     (void) deltaTime;

//     if (!_isServer) {
//         _window->clear();
//         playEvent();
//         _window->draw(_backgroundSprite);
//         // clientLoop();
//     }
//     /*    .this->_window.clear();
//     playEvent(game, game.getEntities());
//     this->_window.draw(background_s);
//     positionSystem.update(
//         game.getEntities(), &this->_window, deltaTime, false);
//     game.updateLocalplayerPosition();
//     renderSystem.update(game.getEntities(), &this->_window, deltaTime, false);
//     game.manageBuffers();
//     bulletSystem.update(game.getEntities(),
//     &this->_window, deltaTime, false); this->_window.display();
//     backgroundAnimation(&background_s, &clockAnim);*/
// }

// void Game::playEvent()
// {
//     sf::Event event;
//     std::stringstream ss;
//     auto &ecsManager = _refGameEngine.getFeature<Engine::Feature::ECSManager>();
//     auto &player = ecsManager.getEntityById(_ClientId);
//     auto velocity = player->getComponent<ecs::VelocityComponent>();

//     while (_window->pollEvent(event)) {
//         if (event.type == sf::Event::Closed) {
//             this->_window->close();
//         }
//         if (event.type == sf::Event::KeyPressed) {
//             if (event.key.code == sf::Keyboard::Up) {
//                 velocity->setVy(-200.0f);
//                 this->playerAnimations(player, "top");
//             } else if (event.key.code == sf::Keyboard::Down) {
//                 velocity->setVy(200.0f);
//                 this->playerAnimations(player, "down");
//             } else if (event.key.code == sf::Keyboard::Right) {
//                 velocity->setVx(200.0f);
//                 this->playerAnimations(player, "right");
//             } else if (event.key.code == sf::Keyboard::Left) {
//                 velocity->setVx(-200.0f);
//                 this->playerAnimations(player, "left");
//             } else if (event.key.code == sf::Keyboard::Space) {
//                 playerShoot(player);
//             }
//         }
//         if (event.type == sf::Event::KeyReleased) {
//             this->playerAnimations(player, "none");
//             if (event.key.code == sf::Keyboard::Up
//                 || event.key.code == sf::Keyboard::Down) {
//                 velocity->setVy(0.0f);
//             }
//             if (event.key.code == sf::Keyboard::Left
//                 || event.key.code == sf::Keyboard::Right) {
//                 velocity->setVx(0.0f);
//             }
//         }
//     }
// }

    // game.setupServer();
    std::cout << "Server mode" << std::endl;
#endif
#ifndef RTYPE_SERVER
    // game.setupClient();
    std::cout << "Client mode" << std::endl;
#endif

    // gameEngine.setTickRate(60);
    // gameEngine.loadFeature<Engine::Feature::ECSManager>();
    // gameEngine.loadFeature<Engine::Feature::EventManager>();
    // gameEngine.loadFeature<Engine::Feature::NetworkingManager>();

    // auto &manager = gameEngine.getFeature<Engine::Feature::ECSManager>();

    // manager.createSubsystem<ecs::BulletSystem>();

    // auto &bulletSub = manager.getSubsystem<ecs::BulletSystem>();
    // auto entity = manager.createEntity();
    // entity->addComponent((bulletSub));

    // gameEngine.mainLoop();
}
