/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "game.hpp"
#include <ctime>
#include <thread>
#include "RenderClass.hpp"
#include "system_network.hpp"

void game::setServerMode(bool mode)
{
    _isServer = mode;
}

int print_help()
{
    std::cout << "USAGE: ./game {IP} {PORT TCP} {PORT UDP}" << std::endl;
    return 0;
}

int main(int argc, char **argv)
{
    game game;
    System::Network::initNetwork();
#if defined(RTYPE_SERVER)
    std::cout << "Je suis un serveur" << std::endl;
    game.setServerMode(true);
#endif
    game.setServerMode(false);

    if (argc != 4)
        return print_help();
    if (!atoi(argv[1]) || !atoi(argv[2]))
        return print_help();
    int portTCP = atoi(argv[2]);
    int portUDP = atoi(argv[3]);
    std::cout << "Connecting to: " << argv[1] << " on port " << portTCP << " "
              << portUDP << std::endl;

    game.createConnection(argv[1], portTCP, portUDP);
    std::thread(&game::receiveMessage, &game).detach();
    std::cout << "game connected" << std::endl;

    game.createPlayer();
    game.getRenderClass().renderWindow(game);
}
