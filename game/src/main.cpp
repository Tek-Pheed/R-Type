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
#include <stdexcept>
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Game.hpp"
#include "system_network.hpp"

[[maybe_unused]]
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

static int prepareGame(
    int argc, const char *argv[], RType::GameInstance &gameInstance)
{
    if constexpr (server) {
        try {
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
        } catch (const System::Network::NetworkException &e) {
            std::cout << CATCH_ERROR_LOCATION
                "Network error while trying to setup server: "
                      << e.what() << std::endl;
            return (84);
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
    gameEngine.verboseLogs = RType::GameInstance::DEBUG_LOGS;
    gameEngine
        .loadFeature<Engine::Feature::LevelManager<RType::GameInstance>>();
    gameEngine.loadFeature<Engine::Feature::AssetManager>();
    gameEngine.loadFeature<Engine::Feature::NetworkingManager>();

    RType::GameInstance gameInstance(gameEngine);
    try {
        int result = 0;
        result = prepareGame(argc, argv, gameInstance);
        if (result != 0) {
            std::cout << "Failed to launch R-Type" << std::endl;
            return (result);
        }
        gameEngine.mainLoop();
    } catch (const std::runtime_error &e) {
        if constexpr (RType::GameInstance::DEBUG_LOGS)
            std::cout << CATCH_ERROR_LOCATION
                "An exception occured during runtime: "
                      << std::endl
                      << e.what() << std::endl;
        return (84);
    }
    return (0);
}
