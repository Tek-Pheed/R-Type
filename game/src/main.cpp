/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cstdint>
#include <exception>
#include <iostream>
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Game.hpp"

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
    gameEngine
        .loadFeature<Engine::Feature::LevelManager<RType::GameInstance>>();
    gameEngine.loadFeature<Engine::Feature::AssetManager>();
    gameEngine.loadFeature<Engine::Feature::NetworkingManager>();

    RType::GameInstance gameInstance(gameEngine);
    try {
        int result = 0;
#if defined(RTYPE_SERVER)
        result = prepareGame(argc, argv, gameInstance, true);
#else
        result = prepareGame(argc, argv, gameInstance, false);
#endif
        if (result != 0)
            return (result);
        gameEngine.mainLoop();
    } catch (const std::exception &e) {
        std::cout << CATCH_ERROR_LOCATION
            "We are sorry, but an unhandled exception occured during "
            "runtime: "
                  << std::endl
                  << e.what() << std::endl;
        return (84);
    }
    return (0);
}
