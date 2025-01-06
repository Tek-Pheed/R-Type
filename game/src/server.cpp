/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server specific functions
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <any>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include "Components.hpp"
#include "EngineNetworking.hpp"
#include "Game.hpp"
#include "GameProtocol.hpp"
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
        deletePlayer((size_t) id);
    }
}

void GameInstance::serverEventPackets(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) arg;
    (void) core;
    std::cout << "Server wakeup on event: " << event << std::endl;
    manageBuffers();
}

void RType::GameInstance::serverHanlderValidateConnection(
    int code, const std::vector<std::string> &tokens)
{
    if (code == Protocol::C_START_UDP && tokens.size() >= 1) {
        ssize_t netClientID = (ssize_t) atoi(tokens[0].c_str());
        if (netClientID >= 0) {
            std::stringstream ss;
            ss << C_AUTH << " OK" << PACKET_END;
            refNetworkManager.sendToOne((size_t) netClientID,
                System::Network::ISocket::Type::TCP, ss.str());
            for (auto &p : getAllPlayers()) {
                auto pos = p.get().getComponent<ecs::PositionComponent>();
                auto pl = p.get().getComponent<ecs::PlayerComponent>();
                if (!pl || !pos) {
                    std::cout << "can't get player" << std::endl;
                    continue;
                }
                std::stringstream sss;
                sss << P_CONN << " " << pl->getPlayerID() << " " << pos->getX()
                    << " " << pos->getY() << PACKET_END;
                refNetworkManager.sendToOne((size_t) netClientID,
                    System::Network::ISocket::Type::TCP, sss.str());
            }
        } else {
            std::cout << "Could not read client ID" << std::endl;
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
    refGameEngine.addEventBinding<GameInstance>(
        Engine::Events::EVENT_OnDataReceived,
        &GameInstance::serverEventPackets, *this);
    refGameEngine.addEventBinding<GameInstance>(
        Engine::Events::EVENT_OnServerNewClient,
        &GameInstance::serverEventNewConn, *this);
    refGameEngine.addEventBinding<GameInstance>(
        Engine::Events::EVENT_OnServerLostClient,
        &GameInstance::serverEventClosedConn, *this);
    auto &level = refEntityManager.createNewLevel("mainLevel");
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::BulletSystem>().initSystem(*this);
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