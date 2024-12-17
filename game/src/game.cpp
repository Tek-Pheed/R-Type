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
//     ecs::PositionSystem &positionSystem = getSubSystem<ecs::PositionSystem>();

//     bulletSystem.update(getEntities(), nullptr, deltaTime_sec, isServer());
//     positionSystem.update(getEntities(), nullptr, deltaTime_sec, isServer());
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
//     std::cout << "Connecting to: " << argv[1] << " on port " << portTCP << " "
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
#include "Engine.hpp"
#include "ecsManager.hpp"

int main(int argc, char **argv) {

    (void) argc;
    (void) argv;

    Engine::Core gameEngine;

    gameEngine.setTickRate(60);
    gameEngine.loadFeature<Engine::Feature::ECSManager>();

    auto &manager = gameEngine.getFeature<Engine::Feature::ECSManager>();



    manager.createSubsystem<ecs::BulletSystem>();


    std::cout << "Bonjour" << std::endl;

}