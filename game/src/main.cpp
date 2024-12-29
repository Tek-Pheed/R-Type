/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <cstdint>
#include <iostream>
#include <memory>
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "GameEvents.hpp"
#include "GameSystems.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "Game.hpp"

void RType::GameInstance::setupClient(
    const std::string &ip, uint16_t tcpPort, uint16_t udpPort)
{
    _isServer = false;
    _ip = ip;
    _tcpPort = tcpPort;
    _udpPort = udpPort;
    _window = std::make_unique<sf::RenderWindow>();
    sf::VideoMode videoMode(
        1280, 720, sf::VideoMode::getDesktopMode().bitsPerPixel);
    _window->create(videoMode, "R-Type");
    _window->setFramerateLimit(refGameEngine.getTickRate());
    if (!_window->isOpen()) {
        throw std::runtime_error("Failed to create the SFML window.");
    }
    loadTexture();
}

static int printServerHelp()
{
    std::cout << "USAGE: ./server [PORT TCP] [PORT UDP]" << std::endl;
    return (84);
}

static int printClientHelp()
{
    std::cout << "USAGE: ./client [IP] [PORT TCP] [PORT UDP]" << std::endl;
    return (84);
}

static int prepareGame(int argc, const char *argv[],
    RType::GameInstance &gameInstance, bool server)
{
    if (server) {
        if (argc == 1) {
            gameInstance.setupServer();
        } else {
            if (argc != 3)
                return (printServerHelp());
            if (!atoi(argv[1]) || !atoi(argv[2]))
                return (printClientHelp());
            uint16_t portTCP = (uint16_t) atoi(argv[1]);
            uint16_t portUDP = (uint16_t) atoi(argv[2]);
            gameInstance.setupServer(portTCP, portUDP);
        }
        std::cout << "R-Type running as server" << std::endl;
    } else {
        if (argc == 1) {
            gameInstance.setupClient();
        } else {
            if (argc != 4)
                return (printClientHelp());
            if (!atoi(argv[2]) || !atoi(argv[3]))
                return (printClientHelp());
            uint16_t portTCP = (uint16_t) atoi(argv[2]);
            uint16_t portUDP = (uint16_t) atoi(argv[3]);
            gameInstance.setupClient(std::string(argv[1]), portTCP, portUDP);
        }
        std::cout << "R-Type running as client" << std::endl;
    }
    return (0);
}

int main(int argc, const char *argv[])
{
    Engine::Core gameEngine;
    gameEngine.setTickRate(RType::GameInstance::REFRESH_RATE);
    gameEngine.loadFeature<Engine::Feature::LevelManager<RType::GameInstance>>();
    gameEngine.loadFeature<Engine::Feature::AssetManager>();
    gameEngine.loadFeature<Engine::Feature::NetworkingManager>();

    RType::GameInstance gameInstance(gameEngine);

    int result = 0;
#if defined(RTYPE_SERVER)
    result = prepareGame(argc, argv, gameInstance, true);
#else
    result = prepareGame(argc, argv, gameInstance, false);
#endif
    if (result != 0)
        return (result);
    gameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_OnTick, &RType::GameInstance::gameUpdate,
        gameInstance);
    gameEngine.mainLoop();
    return (0);
}
