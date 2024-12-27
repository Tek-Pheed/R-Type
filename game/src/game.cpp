/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

// #include <cstdint>
// #include "Networking.hpp"
// #include "Systems.hpp"
// #if defined(WIN32)
//     #define NOMINMAX
// #endif

// #include <ctime>
// #include "RenderClass.hpp"
// #include "game.hpp"
// #include "server.hpp"
// #include "system_network.hpp"

// void game::setServerMode(bool mode)
// {
//     _isServer = mode;
// }

// bool game::isServer(void)
// {
//     return (_isServer);
// }

// int print_help()
// {
//     std::cout << "USAGE: ./game {IP} {PORT TCP} {PORT UDP}" << std::endl;
//     return 0;
// }

// Networking &game::getNetworking()
// {
//     return (_refNetwork);
// }

// void game::gameUpdate(float deltaTime_sec)
// {
//     ecs::BulletSystem &bulletSystem = getSubSystem<ecs::BulletSystem>();
//     ecs::PositionSystem &positionSystem =
//     getSubSystem<ecs::PositionSystem>();

//     bulletSystem.update(getEntities(), nullptr, deltaTime_sec, isServer());
//     positionSystem.update(getEntities(), nullptr, deltaTime_sec,
//     isServer());
// }

// int main(int argc, char **argv)
// {
//     System::Network::initNetwork();
//     Networking net;
//     game game(net);
//     game.setServerMode(false);

// #if defined(RTYPE_SERVER)
//     game.setServerMode(true);
// #endif
//     if (argc != 4)
//         return print_help();
//     if (!atoi(argv[1]) || !atoi(argv[2]))
//         return print_help();
//     int portTCP = atoi(argv[2]);
//     int portUDP = atoi(argv[3]);
//     std::cout << "Connecting to: " << argv[1] << " on port " << portTCP << "
//     "
//               << portUDP << std::endl;

//     if (game.isServer()) {
//         net.setupServer((uint16_t) portTCP, (uint16_t) portUDP);
//         serverLoop(net, game);
//     } else {
//         net.setupClient(
//             (uint16_t) portTCP, (uint16_t) portUDP, std::string(argv[1]));
//         RenderClass render(1280, 720, "R-Type", 60);
//         game.setRenderClass(&render);
//         std::cout << "game connected" << std::endl;
//         game.createPlayer();
//         game.getRenderClass()->renderWindow(game);
//     }
// }

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

Game::Game(Engine::Core &engineRef)
    : refGameEngine(engineRef),
      entityManager(engineRef.getFeature<Engine::Feature::ECSManager<Game>>()),
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
    return (entityManager.getEntityById(_PlayerId));
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
    auto &posSystem = entityManager.getSubsystem<PositionSystem>();
    auto &renderSystem = entityManager.getSubsystem<RenderSystem>();
    auto &bulletSystem = entityManager.getSubsystem<BulletSystem>();

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
    return (entityManager.getEntitiesVect());
}

void Game::playEvent()
{
    sf::Event event;
    std::stringstream ss;
    auto &player = entityManager.getEntityById(_PlayerId);
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
