/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "Components.hpp"
#include "Config.hpp"
#include "Engine.hpp"
#include "Events.hpp"
#include "Factory.hpp"
#include "Game.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "system_network.hpp"

using namespace RType;

void RType::GameInstance::clientHandlerConnection(
    int code, const std::vector<std::string> &tokens)
{
    Factory factory(this);

    switch (code) {
        case Protocol::C_INIT_UDP: {
            if (tokens.size() >= 1) {
                _netClientID = (ssize_t) atoi(tokens[0].c_str());
                if (_netClientID >= 0) {
                    std::stringstream ss;
                    ss << C_START_UDP << " " << _netClientID << PACKET_END;
                    refNetworkManager.sendToAll(
                        System::Network::ISocket::Type::UDP, ss.str());
                } else {
                    std::cout << "Could not read client ID" << std::endl;
                }
            }
            break;
        }
        case Protocol::C_AUTH: {
            if (tokens.size() >= 1) {
                if (tokens[0].starts_with("OK") && _netClientID >= 0) {
                    // We can create the player here, or wait and create it
                    // later

                    std::cout << "Build player with id:" << _netClientID
                              << std::endl;
                    factory.buildPlayer(true, (size_t) _netClientID);
                } else {
                    std::cout << "The connection failed." << std::endl;
                }
            }
            break;
        }

        default: break;
    }
}

void RType::GameInstance::connectToGame()
{
    if (_isConnectedToServer)
        return;
    auto currentLevel = refEntityManager.getCurrentLevelName();
    try {
        size_t count = 0;

        for (auto &entity : _inputList) {
            count++;
            auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

            if (!text)
                continue;

            if (text->getStr().empty())
                continue;

            switch (count) {
                case 2: _ip = text->getStr(); break;
                case 3:
                    _tcpPort = (uint16_t) std::atoi(text->getStr().c_str());
                    break;
                case 4:
                    _udpPort = (uint16_t) std::atoi(text->getStr().c_str());
                    break;
                default: break;
            }
        }

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
    refGameEngine.setTickRate(CLIENT_REFRESH_RATE);
    _window = std::make_unique<sf::RenderWindow>();
    sf::VideoMode videoMode(sf::VideoMode::getDesktopMode().width,
        sf::VideoMode::getDesktopMode().height,
        sf::VideoMode::getDesktopMode().bitsPerPixel);
    _window->create(videoMode, "R-Type", sf::Style::Fullscreen);
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
    loadAssets();
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
    Config config("config.cfg");
    EventManager event_manager(this);
    Factory factory(this);

    bool autoFireEnabled = config.getAutoFireConfig();

    if (hasLocalPlayer() && autoFireEnabled
        && this->_autoFireClock.getElapsedTime().asSeconds() >= 1.0f) {
        if (_netClientID >= 0) {
            factory.buildBulletFromPlayer((size_t) _netClientID);
            this->_autoFireClock.restart();
        }
    }

    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window->close();
            refGameEngine.stop();
        }
        if (event.type == sf::Event::KeyPressed) {
            event_manager.keyPressed(event);
        }
        if (hasLocalPlayer() && event.type == sf::Event::KeyReleased) {
            event_manager.keyReleased(event);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            event_manager.mouseClicked();
        }
    }
}
