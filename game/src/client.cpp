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
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include "Components.hpp"
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
                    if constexpr (RType::GameInstance::DEBUG_LOGS)
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
                    if constexpr (RType::GameInstance::DEBUG_LOGS)
                        std::cout << "Build player with id:" << _netClientID
                                  << std::endl;
                    factory.buildPlayer(
                        true, (size_t) _netClientID, playerName);
                } else {
                    if constexpr (RType::GameInstance::DEBUG_LOGS)
                        std::cout << "The connection failed." << std::endl;
                }
            }
            break;
        }

        default: break;
    }
}

int RType::GameInstance::countTxtFiles(const std::string &path)
{
    int count = 0;

    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (entry.path().extension() == ".txt") {
            count++;
        }
    }
    return count;
}

void RType::GameInstance::connectToGame()
{
    if (_isConnectedToServer)
        return;
    auto currentLevel = refEntityManager.getCurrentLevelName();
    try {
        size_t count = 0;

        for (auto &entity : inputList) {
            count++;
            auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

            if (!text)
                continue;

            if (text->getStr().empty() || text->getStr() == "IP ADDRESS")
                continue;

            switch (count) {
                case 1: playerName = text->getStr(); break;
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

        int TempnbTxtFiles = countTxtFiles("assets/levels");
        this->nbTxtFiles = TempnbTxtFiles;
        std::stringstream ss;

        ss << L_SENDLEVELS << " " << nbTxtFiles << PACKET_END;

        refNetworkManager.sendToAll(
            System::Network::ISocket::Type::TCP, ss.str());

        levels.buildLobby();

        _playerEntityID = -1;
        _isConnectedToServer = true;

    } catch (const System::Network::NetworkException &e) {
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
        setHealthId(static_cast<int>(getNewId()));
        factory.buildText(static_cast<size_t>(getHealthId()), 0, 0, text,
            sf::Color::White, 48);
    } catch (const std::exception &e) {
        levels.buildMainMenu();
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
    levels.buildMainMenu();
}

void RType::GameInstance::setupClient(
    const std::string &ip, uint16_t tcpPort, uint16_t udpPort)
{
    _ip = ip;
    _tcpPort = tcpPort;
    _udpPort = udpPort;
    refGameEngine.setTickRate(CLIENT_REFRESH_RATE);
    _window = std::make_unique<sf::RenderWindow>();

    auto res = gameConfig.getKeyFromConfig("RESOLUTION");
    std::stringstream resStream(res);
    std::string resX;
    std::string resY;

    std::getline(resStream, resX, 'x');
    std::getline(resStream, resY, ' ');

    WinScaleX = (size_t) std::atoi(resX.c_str());
    WinScaleY = (size_t) std::atoi(resY.c_str());

    sf::VideoMode videoMode(
        (unsigned int) WinScaleX, (unsigned int) WinScaleY);
    _window->create(
        videoMode, "R-Type", sf::Style::Titlebar | sf::Style::Close);
    _window->setFramerateLimit(refGameEngine.getTickRate());
    if (!_window->isOpen()) {
        throw std::runtime_error(
            THROW_ERROR_LOCATION "Failed to create the SFML window.");
    }
    WinScaleX = _window->getSize().x;
    WinScaleY = _window->getSize().y;
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
    levels.buildMainMenu();
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

    bool autoFireEnabled = gameConfig.getAutoFireConfig();

    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window->close();
            refNetworkManager.stopNetworking();
            refGameEngine.stop();
        }
        if (event.type == sf::Event::KeyPressed) {
            event_manager.keyPressed(event);
            if (hasLocalPlayer()) {
                if (_gameStarted) {
                    auto &player = getLocalPlayer();
                    auto bonus = player.getComponent<ecs::BonusComponent>();
                    if (!autoFireEnabled
                        && event.key.code == sf::Keyboard::Space
                        && this->_fireClock.getElapsedTime().asSeconds()
                            >= (bonus->getBonus() == ecs::Bonus::RAPIDFIRE
                                    ? 0.10f
                                    : 0.25f)) {
                        if (_netClientID >= 0) {
                            factory.buildBulletFromPlayer(
                                (size_t) _netClientID);
                            this->_fireClock.restart();
                        }
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

    if (hasLocalPlayer()) {
        auto &player = getLocalPlayer();
        auto bonus = player.getComponent<ecs::BonusComponent>();

        if (bonus->getBonus() == ecs::Bonus::NONE) {
            this->_bonusClock.restart();
        } else if (bonus->getBonus() != ecs::Bonus::NONE
            && this->_bonusClock.getElapsedTime().asSeconds() >= 15.0f) {
            bonus->setBonus(ecs::Bonus::NONE);
            this->_bonusClock.restart();
        }

        if (autoFireEnabled && _gameStarted
            && this->_fireClock.getElapsedTime().asSeconds()
                >= (bonus->getBonus() == ecs::Bonus::RAPIDFIRE ? 0.10f
                                                               : 0.25f)) {
            if (_netClientID >= 0) {
                factory.buildBulletFromPlayer((size_t) _netClientID);
                this->_fireClock.restart();
            }
        }
    }
}
