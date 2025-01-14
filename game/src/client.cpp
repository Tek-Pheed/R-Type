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
#include <mutex>
#include <sstream>
#include <string>
#include "Components.hpp"
#include "Config.hpp"
#include "Engine.hpp"
#include "EngineNetworking.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include "system_network.hpp"

using namespace RType;

void RType::GameInstance::clientStartLevel()
{
    _gameStarted = true;
    std::stringstream ss;
    ss << L_STARTGAME << " " << _netClientID << PACKET_END;
    refNetworkManager.sendToAll(System::Network::ISocket::Type::TCP, ss.str());
}

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
                    std::unique_lock lock(_gameLock);
                    _factory.buildPlayer(
                        true, (size_t) _netClientID, _playerName);
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

            if (text->getStr().empty() || text->getStr() == "IP ADRESS")
                continue;

            switch (count) {
                case 1: _playerName = text->getStr(); break;
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

        auto songEntity =
            refEntityManager.getPersistentLevel()
                .findEntitiesByComponent<ecs::MusicComponent<sf::Sound>>()[0];
        auto currentSong =
            songEntity.get().getComponent<ecs::MusicComponent<sf::Sound>>();
        auto &newMusic =
            refAssetManager.getAsset<sf::SoundBuffer>(Asset::LOBBY_SONG);

        if (currentSong->getMusicType().getStatus()
            == sf::SoundSource::Playing) {
            currentSong->getMusicType().stop();
            currentSong->getMusicType().setBuffer(newMusic);
            currentSong->getMusicType().play();
        }

        // Prepare level
        auto &level = refEntityManager.createNewLevel("mainLevel");
        level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
        level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
        level.createSubsystem<GameSystems::BackgroundSystem>().initSystem(
            *this);
        level.createSubsystem<GameSystems::BulletSystem>().initSystem(*this);
        level.createSubsystem<GameSystems::HitboxSystem>().initSystem(*this);
        level.createSubsystem<GameSystems::HealthSystem>().initSystem(*this);
        refEntityManager.switchLevel("mainLevel", false);

        levelLobbyMenu();

        _playerEntityID = -1;
        _isConnectedToServer = true;

    } catch (const std::exception &e) {
        std::cout << CATCH_ERROR_LOCATION
                  << "Failed to connect to server: IP=" << _ip
                  << " TCP=" << _tcpPort << " UDP=" << _udpPort
                  << " with error: " << e.what() << std::endl;
        refEntityManager.switchLevel(currentLevel);
    }
}

void RType::GameInstance::launchGame()
{
    std::unique_lock lock(_gameLock);
    try {
        for (auto &entity : refEntityManager.getCurrentLevel().getEntities()) {
            auto render = entity.get().getComponent<ecs::RenderComponent>();
            if (!render)
                continue;
            if (render->getObjectType()
                    == ecs::RenderComponent::ObjectType::BUTTON
                || render->getObjectType()
                    == ecs::RenderComponent::ObjectType::TEXT) {
                refEntityManager.getCurrentLevel().markEntityForDeletion(
                    entity.get().getID());
            }
        }
        std::string text = "Health: "
            + std::to_string(getLocalPlayer()
                                 .getComponent<ecs::HealthComponent>()
                                 ->getHealth());
        setHealthId(getNewId());
        _factory.buildText(getHealthId(), 0, 0, text);
    } catch (const std::exception &e) {
        levelMainMenu();
    }
}

void RType::GameInstance::clientHandleDisconnected(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) event;
    (void) core;
    (void) arg;

    refNetworkManager.stopNetworking();
    std::unique_lock lock(_gameLock);

    _gameStarted = false;
    _isConnectedToServer = false;
    _playerEntityID = -1;
    refNetworkManager.disconnectClient(1);
    std::cout << CATCH_ERROR_LOCATION
              << "You are now disconnected from the game server, maybe the "
                 "connection was unstable."
              << std::endl;
    refEntityManager.deleteAllLevel();
    levelMainMenu();
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
        GameInstance::RESOLUTION_X, GameInstance::RESOLUTION_Y);
    _window->create(
        videoMode, "R-Type", sf::Style::Titlebar | sf::Style::Close);
    _window->setFramerateLimit(refGameEngine.getTickRate());
    if (!_window->isOpen()) {
        throw std::runtime_error(
            THROW_ERROR_LOCATION "Failed to create the SFML window.");
    }
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_OnTick, &RType::GameInstance::gameTick, *this);
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_BeforeTick, &RType::GameInstance::gamePreTick,
        *this);
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_PostTick, &RType::GameInstance::gamePostTick,
        *this);
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_DisconnectedFromServer,
        &RType::GameInstance::clientHandleDisconnected, *this);
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
    std::unique_lock lock(_gameLock);
    sf::Event event;
    std::stringstream ss;
    EventManager event_manager(*this);

    bool autoFireEnabled = _gameConfig.getAutoFireConfig();

    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window->close();
            refNetworkManager.stopNetworking();
            refGameEngine.stop();
        }
        if (event.type == sf::Event::KeyPressed) {
            event_manager.keyPressed(event);
            if (!_gameStarted) {
                if (event.key.code == sf::Keyboard::Tab) {
                    clientStartLevel();
                }
            }
            if (hasLocalPlayer() && _gameStarted) {
                auto &player = getLocalPlayer();
                auto velocity = player.getComponent<ecs::VelocityComponent>();
                if (!autoFireEnabled && event.key.code == sf::Keyboard::Space
                    && this->_fireClock.getElapsedTime().asSeconds() >= 0.5f) {
                    if (_netClientID >= 0) {
                        _factory.buildBulletFromPlayer((size_t) _netClientID);
                        this->_fireClock.restart();
                    }
                }
            }
        }
        if (hasLocalPlayer() && event.type == sf::Event::KeyReleased) {
            event_manager.keyReleased(event);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            event_manager.mouseClicked();
        }
    }
    if (hasLocalPlayer() && autoFireEnabled && _gameStarted
        && this->_fireClock.getElapsedTime().asSeconds() >= 0.5f) {
        if (_netClientID >= 0) {
            _factory.buildBulletFromPlayer((size_t) _netClientID);
            this->_fireClock.restart();
        }
    }
}
