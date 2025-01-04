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
#include "Components.hpp"
#include "Engine.hpp"
#include "Game.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "system_network.hpp"
#include "Event.hpp"

using namespace RType;

void RType::GameInstance::clientHandlerConnection(
    int code, const std::vector<std::string> &tokens)
{
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
                    buildPlayer(true, (size_t) _netClientID);
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
    sf::VideoMode videoMode(
        sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, sf::VideoMode::getDesktopMode().bitsPerPixel);
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

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

void GameInstance::playEvent()
{
    sf::Event event;
    //Event evt;
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
                } else if (event.key.code == sf::Keyboard::Down) {
                    velocity->setVy(200.0f);
                } else if (event.key.code == sf::Keyboard::Right) {
                    velocity->setVx(200.0f);
                } else if (event.key.code == sf::Keyboard::Left) {
                    velocity->setVx(-200.0f);
                } else if (event.key.code == sf::Keyboard::Space) {
                    if (_netClientID >= 0)
                        playerShoot((size_t) _netClientID);
                }
            }
        }
        if (hasLocalPlayer() && event.type == sf::Event::KeyReleased) {
            auto &player = getLocalPlayer();
            auto velocity = player.getComponent<ecs::VelocityComponent>();
            if (event.key.code == sf::Keyboard::Up
                || event.key.code == sf::Keyboard::Down) {
                velocity->setVy(0.0f);
            }
            if (event.key.code == sf::Keyboard::Left
                || event.key.code == sf::Keyboard::Right) {
                velocity->setVx(0.0f);
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            //evt.mouseClicked();
            for (auto &entity : refEntityManager.getCurrentLevel().getEntities()) {
                auto button = entity.get().getComponent<ecs::RenderComponent>();
                if (button->getObjectType() == ecs::RenderComponent::ObjectType::BUTTON) {
                    sf::Mouse mouse;
                    auto rectangle = entity.get().getComponent<ecs::RectangleComponent<sf::RectangleShape>>();
                    if (rectangle) {
                        sf::FloatRect currentHover = rectangle->getRectangle().getGlobalBounds();
                        if (currentHover.contains((float) mouse.getPosition().x, (float) mouse.getPosition().y)) {
                            auto text = entity.get().getComponent<ecs::TextComponent<sf::Text>>();
                            switch (str2int(text->getStr().c_str())) {
                                case str2int("Play"):
                                    connectToGame();
                                    break;
                                case str2int("Exit"):
                                    _window->close();
                                    refGameEngine.stop();
                                    break;
                                default: break;
                            }
                        }
                    }
                }
            }
        }
    }
}
