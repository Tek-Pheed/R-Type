/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server specific functions
*/

#include "Engine.hpp"
#if defined(WIN32)
    #define NOMINMAX
#endif

#include <any>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <mutex>
#include <sstream>
#include "Components.hpp"
#include "EngineNetworking.hpp"
#include "Game.hpp"
#include "GameProtocol.hpp"
#include "SFML/Graphics/Text.hpp"
#include "system_network.hpp"

using namespace RType;

void GameInstance::serverEventNewConn(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) core;
    size_t newID = std::any_cast<size_t>(arg);
    std::cout << "Server wakeup on event: " << event << std::endl;
    std::stringstream ss;
    ss << C_INIT_UDP << " " << newID << PACKET_END;
    refNetworkManager.sendToOne(
        newID, System::Network::ISocket::TCP, ss.str());
}

void GameInstance::serverEventClosedConn(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) core;
    ssize_t id = std::any_cast<ssize_t>(arg);
    std::cout << "Server wakeup on event: " << event << std::endl;
    std::stringstream ss;
    if (id >= 0) {
        try {
            deletePlayer((size_t) id);
            refNetworkManager.disconnectClient((size_t) id);
        } catch (const std::exception &e) {
            std::cout << CATCH_ERROR_LOCATION "Could not delete player: " << id
                      << ", maybe it failed to connect." << std::endl;
        }
    }
}

void RType::GameInstance::serverSendGameState(size_t clientID)
{
    std::unique_lock lock(_gameLock);

    std::cout << "Sending game state to new player: " << clientID << std::endl;
    for (auto &p : getAllPlayers()) {
        auto pos = p.get().getComponent<ecs::PositionComponent>();
        auto pl = p.get().getComponent<ecs::PlayerComponent>();
        std::string n = DEFAULT_PLAYER_NAME;

        if (_isServer) {
            auto nm = p.get().getComponent<ecs::TextComponent<std::string>>();
            if (nm)
                n = nm->getText();
        } else {
            auto nm = p.get().getComponent<ecs::TextComponent<sf::Text>>();
            if (nm)
                nm->getText();
        }
        if (!pl || !pos) {
            std::cout << "serverSendGameState: Failed to get player"
                      << std::endl;
            continue;
        }
        std::stringstream sss;
        sss << P_CONN << " " << pl->getPlayerID() << " " << pos->getX() << " "
            << pos->getY() << " " << n << PACKET_END;
        refNetworkManager.sendToOne(
            clientID, System::Network::ISocket::Type::TCP, sss.str());
    }
    for (auto &e : refEntityManager.getCurrentLevel()
             .findEntitiesByComponent<ecs::EnemyComponent>()) {
        auto pos = e.get().getComponent<ecs::PositionComponent>();
        auto ec = e.get().getComponent<ecs::EnemyComponent>();
        auto hl = e.get().getComponent<ecs::HealthComponent>();
        if (!ec || !pos) {
            std::cout << "serverSendGameState: Failed to get enemy"
                      << std::endl;
            continue;
        }
        std::stringstream sss;
        sss << E_SPAWN << " " << ec->getEnemyID() << " " << ec->getType()
            << " " << pos->getX() << " " << pos->getY() << " "
            << hl->getHealth() << " " << ec->getWave() << PACKET_END;
        refNetworkManager.sendToOne(
            clientID, System::Network::ISocket::Type::TCP, sss.str());
    }
    std::stringstream s;
    if (_musicName != "") {
        s << M_MUSIC << " " << _musicName << " " << PACKET_END;
        refNetworkManager.sendToOne(
            clientID, System::Network::ISocket::Type::TCP, s.str());
    }
    s.clear();
    if (_bgName != "") {
        s << M_BG << " " << _bgName << " " << PACKET_END;
        refNetworkManager.sendToOne(
            clientID, System::Network::ISocket::Type::TCP, s.str());
    }
    s.clear();
    s << M_WAVE << " " << currentWave << PACKET_END;
    refNetworkManager.sendToOne(
        clientID, System::Network::ISocket::Type::TCP, s.str());
    if (_gameStarted) {
        std::stringstream sss;
        sss << L_STARTGAME << " " << 0 << PACKET_END;
        refNetworkManager.sendToOne(
            clientID, System::Network::ISocket::Type::TCP, sss.str());
    }
}

void RType::GameInstance::serverHanlderValidateConnection(
    int code, const std::vector<std::string> &tokens)
{
    std::unique_lock lock(_gameLock);

    if (code == Protocol::C_START_UDP && tokens.size() >= 1) {
        ssize_t netClientID = (ssize_t) atoi(tokens[0].c_str());
        if (netClientID >= 0) {
            _clientTicks[(size_t) netClientID] = 0;
            std::stringstream ss;
            if (getAllPlayers().size() + 1 > _maxPlayers) {
                ss << C_AUTH << " KO" << PACKET_END;
                refNetworkManager.sendToOne((size_t) netClientID,
                    System::Network::ISocket::Type::TCP, ss.str());
                refNetworkManager.disconnectClient((size_t) netClientID);
            } else {
                ss << C_AUTH << " OK" << PACKET_END;
                refNetworkManager.sendToOne((size_t) netClientID,
                    System::Network::ISocket::Type::TCP, ss.str());
                serverSendGameState((size_t) netClientID);
            }
        } else {
            std::cout
                << "serverHanlderValidateConnection: Could not read client ID"
                << std::endl;
        }
    }
}

void GameInstance::setupServer(uint16_t tcpPort, uint16_t udpPort)
{
    _isServer = true;
    _tcpPort = tcpPort;
    _udpPort = udpPort;
    refNetworkManager.setupServer<PacketHandler>(_tcpPort, _udpPort);
    refGameEngine.setTickRate(SERVER_REFRESH_RATE);
    refGameEngine.addEventBinding<RType::GameInstance>(
        Engine::Events::EVENT_OnTick, &RType::GameInstance::gameTick, *this);
    refGameEngine.addEventBinding<GameInstance>(
        Engine::Events::EVENT_OnServerNewClient,
        &GameInstance::serverEventNewConn, *this);
    refGameEngine.addEventBinding<GameInstance>(
        Engine::Events::EVENT_OnServerLostClient,
        &GameInstance::serverEventClosedConn, *this);
    refGameEngine.addEventBinding<GameInstance>(
        Engine::Events::EVENT_PostTick, &GameInstance::gamePostTick, *this);
    auto &level = refEntityManager.createNewLevel("mainLevel");
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::BulletSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::HealthSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::HitboxSystem>().initSystem(*this);
    refEntityManager.switchLevel("mainLevel");
}

bool GameInstance::isConnectedToServer()
{
    return this->_isConnectedToServer;
}

ssize_t GameInstance::getNetClientID()
{
    return this->_netClientID;
}
