/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Graphics.hpp>
#include <memory>
#include "Game.hpp"
#include "SFML/Window/VideoMode.hpp"

using namespace RType;

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
    createPersistentLevel();
    levelMainMenu();

    // Connect to server only when player select a game
    // networkManager.setupClient<RType::PacketHandler>(_tcpPort, _udpPort,
    // _ip);
}

sf::RenderWindow &GameInstance::getWindow()
{
    return *_window;
}


