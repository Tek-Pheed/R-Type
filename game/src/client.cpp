/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include "Engine.hpp"
#include "GameSystems.hpp"
#if defined(WIN32)
    #define NOMINMAX
#endif

#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include "Game.hpp"
#include "GameNetwork.hpp"
#include "SFML/Window/VideoMode.hpp"

using namespace RType;

int RType::GameInstance::clientManageBuffers()
{
    if (!_isConnectedToServer)
        return (-1);

    auto packets = refNetworkManager.readAllPackets();
    if (packets.size() == 0)
        return 0;
    for (auto buffer : packets) {
        std::string codeStr = std::string(buffer).substr(0, 3);
        int code = atoi(codeStr.c_str());
        int code_int = is_code_valid(code);
        std::vector<std::string> tokens;
        if (code_int == -1) {
            return -1;
        }
        std::string str = buffer.substr(4, buffer.size() - 4);
        std::istringstream ss(str);
        std::string token;
        std::cout << "Buffer: " << buffer << std::endl;
        while (std::getline(ss, token, ' ')) {
            tokens.push_back(token);
        }
        // switch (code_int) {
        //     case 0: handlePlayer(code, tokens); break;
        //     case 1: handle_enemy(code, tokens); break;
        //     case 2: handle_terrain(code, tokens); break;
        //     case 3: handle_mechs(code, tokens); break;
        //     case 9: handle_connection(code, tokens); break;
        //     default: break;
        // }
    }
    return 0;
}

void RType::GameInstance::connectToGame()
{
    if (_isConnectedToServer)
        return;
    auto currentLevel = refEntityManager.getCurrentLevelName();
    try {
        refNetworkManager.setupClient<RType::PacketHandler>(
            _tcpPort, _udpPort, _ip);

        // Prepare level
        auto &level = refEntityManager.createNewLevel("mainRemoteLevel");
        level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
        level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
        level.createSubsystem<GameSystems::BackgroundSystem>().initSystem(
            *this);
        level.createSubsystem<GameSystems::BulletSystem>().initSystem(*this);
        refEntityManager.switchLevel("mainRemoteLevel", false);
        _playerEntityID = -1;
        _isConnectedToServer = true;
    } catch (const std::exception &e) {
        std::cout << "Failed to connect to server: IP=" << _ip
                  << " TCP=" << _tcpPort << " UDP=" << _udpPort
                  << " with error: " << e.what() << std::endl;
        refEntityManager.switchLevel(currentLevel);
    }
}

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
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_OnTick, &RType::GameInstance::gameTick, *this);
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_BeforeTick, &RType::GameInstance::gamePreTick,
        *this);
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_PostTick, &RType::GameInstance::gamePostTick,
        *this);
    loadTexture();
    createPersistentLevel();
    levelMainMenu();
}

sf::RenderWindow &GameInstance::getWindow()
{
    return *_window;
}

void GameInstance::playEvent()
{
    sf::Event event;
    std::stringstream ss;

    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window->close();
            refGameEngine.stop();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (hasLocalPlayer()) {
                auto &player = getLocalPlayer();
                auto velocity = player.getComponent<ecs::VelocityComponent>();
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
            if (event.key.code == sf::Keyboard::Enter) {
                connectToGame();
            }
        }
        if (hasLocalPlayer() && event.type == sf::Event::KeyReleased) {
            auto &player = getLocalPlayer();
            auto velocity = player.getComponent<ecs::VelocityComponent>();
            this->playerAnimations(getLocalPlayer(), "none");
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
