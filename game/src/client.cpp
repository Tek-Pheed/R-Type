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
#include "Engine.hpp"
#include "Game.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "system_network.hpp"
#include "Event.hpp"
#include "Config.hpp"

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

sf::Keyboard::Key getKeyFromString(const std::string &keyStr)
{
    static const std::pair<std::string, sf::Keyboard::Key> keyMap[] = {
        {"sf::Keyboard::A", sf::Keyboard::A}, {"sf::Keyboard::B", sf::Keyboard::B}, {"sf::Keyboard::C", sf::Keyboard::C}, {"sf::Keyboard::D", sf::Keyboard::D},
        {"sf::Keyboard::E", sf::Keyboard::E}, {"sf::Keyboard::F", sf::Keyboard::F}, {"sf::Keyboard::G", sf::Keyboard::G}, {"sf::Keyboard::H", sf::Keyboard::H},
        {"sf::Keyboard::I", sf::Keyboard::I}, {"sf::Keyboard::J", sf::Keyboard::J}, {"sf::Keyboard::K", sf::Keyboard::K}, {"sf::Keyboard::L", sf::Keyboard::L},
        {"sf::Keyboard::M", sf::Keyboard::M}, {"sf::Keyboard::N", sf::Keyboard::N}, {"sf::Keyboard::O", sf::Keyboard::O}, {"sf::Keyboard::P", sf::Keyboard::P},
        {"sf::Keyboard::Q", sf::Keyboard::Q}, {"sf::Keyboard::R", sf::Keyboard::R}, {"sf::Keyboard::S", sf::Keyboard::S}, {"sf::Keyboard::T", sf::Keyboard::T},
        {"sf::Keyboard::U", sf::Keyboard::U}, {"sf::Keyboard::V", sf::Keyboard::V}, {"sf::Keyboard::W", sf::Keyboard::W}, {"sf::Keyboard::X", sf::Keyboard::X},
        {"sf::Keyboard::Y", sf::Keyboard::Y}, {"sf::Keyboard::Z", sf::Keyboard::Z},
        {"sf::Keyboard::Num0", sf::Keyboard::Num0}, {"sf::Keyboard::Num1", sf::Keyboard::Num1}, {"sf::Keyboard::Num2", sf::Keyboard::Num2},
        {"sf::Keyboard::Num3", sf::Keyboard::Num3}, {"sf::Keyboard::Num4", sf::Keyboard::Num4}, {"sf::Keyboard::Num5", sf::Keyboard::Num5},
        {"sf::Keyboard::Num6", sf::Keyboard::Num6}, {"sf::Keyboard::Num7", sf::Keyboard::Num7}, {"sf::Keyboard::Num8", sf::Keyboard::Num8},
        {"sf::Keyboard::Num9", sf::Keyboard::Num9},
        {"sf::Keyboard::Up", sf::Keyboard::Up}, {"sf::Keyboard::Down", sf::Keyboard::Down},
        {"sf::Keyboard::Left", sf::Keyboard::Left}, {"sf::Keyboard::Right", sf::Keyboard::Right},
        {"sf::Keyboard::Escape", sf::Keyboard::Escape}, {"sf::Keyboard::Space", sf::Keyboard::Space},
        {"sf::Keyboard::Return", sf::Keyboard::Return}, {"sf::Keyboard::Tab", sf::Keyboard::Tab},
        {"sf::Keyboard::BackSpace", sf::Keyboard::BackSpace}, {"sf::Keyboard::Delete", sf::Keyboard::Delete},
        {"sf::Keyboard::Home", sf::Keyboard::Home}, {"sf::Keyboard::End", sf::Keyboard::End},
        {"sf::Keyboard::PageUp", sf::Keyboard::PageUp}, {"sf::Keyboard::PageDown", sf::Keyboard::PageDown}
    };

    for (const auto& pair : keyMap) {
        if (keyStr == pair.first) {
            return pair.second;
        }
    }

    return sf::Keyboard::Unknown;
}

void GameInstance::playEvent()
{
    sf::Event event;
    std::stringstream ss;
    Config config("config.cfg");
    sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;

    std::string moveUpKeyString = config.getKeyFromConfig("MOVE_UP");
    std::string moveRightKeyString = config.getKeyFromConfig("MOVE_RIGHT");
    std::string moveLeftKeyString = config.getKeyFromConfig("MOVE_LEFT");
    std::string moveDownKeyString = config.getKeyFromConfig("MOVE_DOWN");

    sf::Keyboard::Key moveUpKey = getKeyFromString(moveUpKeyString);
    sf::Keyboard::Key moveRightKey = getKeyFromString(moveRightKeyString);
    sf::Keyboard::Key moveLeftKey = getKeyFromString(moveLeftKeyString);
    sf::Keyboard::Key moveDownKey = getKeyFromString(moveDownKeyString);

    bool autoFireEnabled = config.getAutoFireConfig();

    if (autoFireEnabled && this->_autoFireClock.getElapsedTime().asSeconds() >= 1.0f) {
        if (_netClientID >= 0) {
            playerShoot((size_t)_netClientID);
            this->_autoFireClock.restart();
        }
    }

    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window->close();
            refGameEngine.stop();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (this->_isSettingsUpButtonClicked) {
                keyPressed = event.key.code;
                handleConfigButtons(keyPressed, 0);
                this->_isSettingsUpButtonClicked = false;
            } else if (this->_isSettingsRightButtonClicked) {
                keyPressed = event.key.code;
                handleConfigButtons(keyPressed, -1);
                this->_isSettingsRightButtonClicked = false;
            } else if (this->_isSettingsLeftButtonClicked) {
                keyPressed = event.key.code;
                handleConfigButtons(keyPressed, -2);
                this->_isSettingsLeftButtonClicked = false;
            } else if (this->_isSettingsDownButtonClicked) {
                keyPressed = event.key.code;
                handleConfigButtons(keyPressed, -3);
                this->_isSettingsDownButtonClicked = false;
            }
            if (hasLocalPlayer()) {
                auto &player = getLocalPlayer();
                auto velocity = player.getComponent<ecs::VelocityComponent>();
                if (event.key.code == moveUpKey) {
                    velocity->setVy(-200.0f);
                } else if (event.key.code == moveDownKey) {
                    velocity->setVy(200.0f);
                } else if (event.key.code == moveRightKey) {
                    velocity->setVx(200.0f);
                } else if (event.key.code == moveLeftKey) {
                    velocity->setVx(-200.0f);
                } else if (event.key.code == sf::Keyboard::Escape) {
                    // TO DO : BE ABLE TO GO BACK TO MENU IN GAME
                    //refEntityManager.switchLevel("mainMenu");
                } else if (!autoFireEnabled && event.key.code == sf::Keyboard::Space) {
                    if (_netClientID >= 0)
                        playerShoot((size_t) _netClientID);
                }
            }
        }
        if (hasLocalPlayer() && event.type == sf::Event::KeyReleased) {
            auto &player = getLocalPlayer();
            auto velocity = player.getComponent<ecs::VelocityComponent>();
            if (event.key.code == moveUpKey
                || event.key.code == moveDownKey) {
                velocity->setVy(0.0f);
            }
            if (event.key.code == moveLeftKey
                || event.key.code == moveRightKey) {
                velocity->setVx(0.0f);
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = _window->mapPixelToCoords(sf::Mouse::getPosition(*_window));

            for (auto &entity : refEntityManager.getCurrentLevel().getEntities()) {
                auto button = entity.get().getComponent<ecs::RenderComponent>();
                if (!button || button->getObjectType() != ecs::RenderComponent::ObjectType::BUTTON)
                    continue;

                auto rectangle = entity.get().getComponent<ecs::RectangleComponent<sf::RectangleShape>>();
                if (!rectangle)
                    continue;

                sf::FloatRect currentHover = rectangle->getRectangle().getGlobalBounds();
                if (currentHover.contains(mousePos)) {
                    auto text = entity.get().getComponent<ecs::TextComponent<sf::Text>>();
                    if (!text)
                        continue;

                    switch (str2int(text->getStr().c_str())) {
                        case str2int("PLAY"):
                            connectToGame();
                            break;
                        case str2int("EXIT"):
                            _window->close();
                            refGameEngine.stop();
                            break;
                        case str2int("SETTINGS"):
                            levelSettingsMenu();
                            break;
                        case str2int("BACK"):
                            levelMainMenu();
                            break;
                        default:
                            if (text->getStr().find("MOVE_UP") == 0) {
                                this->_isSettingsUpButtonClicked = true;
                            } else if (text->getStr().find("MOVE_RIGHT") == 0) {
                                this->_isSettingsRightButtonClicked = true;
                            } else if (text->getStr().find("MOVE_LEFT") == 0) {
                                this->_isSettingsLeftButtonClicked = true;
                            } else  if (text->getStr().find("MOVE_DOWN") == 0) {
                                this->_isSettingsDownButtonClicked = true;
                            } else if (text->getStr().find("AUTO_FIRE") == 0) {
                                bool currentAutoFireValue = config.getAutoFireConfig();
                                std::string newAutoFireValue = currentAutoFireValue ? "false" : "true";
                                handleAutoFireButton(newAutoFireValue);
                            }
                            break;
                    }
                }
            }
        }
    }
}
