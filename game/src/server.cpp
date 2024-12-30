/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server specific functions
*/

#include <cstdlib>
#if defined(WIN32)
    #define NOMINMAX
#endif

#include <any>
#include <cstddef>
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
    refNetworkManager.sendToOne(
        newID, System::Network::ISocket::TCP, serverInitUDP(newID));
}

void GameInstance::serverEventClosedConn(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) arg;
    (void) core;
    std::cout << "Server wakeup on event: " << event << std::endl;
}

void GameInstance::serverEventPackets(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) arg;
    (void) core;
    std::cout << "Server wakeup on event: " << event << std::endl;
    serverManageBuffers();
    // refNetworkManager.sendToOne(size_t id, System::Network::ISocket::Type
    // socketType, const std::string &buffer)
}

void RType::GameInstance::serverHandlePlayer(
    int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case P_CONN: {
            if (tokens.size() >= 3) {
                auto &pl = buildPlayer(true, (size_t) std::atoi(tokens[0].c_str()));
                auto pos = pl.getComponent<ecs::PositionComponent>();
                pos->setX((float) std::atoi(tokens[1].c_str()));
                pos->setX((float) std::atoi(tokens[2].c_str()));
            }
            break;
        }
        default: break;
    }
}

void RType::GameInstance::serverHanlderValidateConnection(
    int code, const std::vector<std::string> &tokens)
{
    if (code == Protocol::C_START_UDP && tokens.size() >= 1) {
        ssize_t netClientID = (ssize_t) atoi(tokens[0].c_str());
        if (netClientID >= 0) {
            refNetworkManager.sendToOne((size_t) netClientID,
                System::Network::ISocket::Type::TCP, serverConfirmUDP(true));
            for (auto &p : getAllPlayers()) {
                auto pos = p.get().getComponent<ecs::PositionComponent>();
                auto pl = p.get().getComponent<ecs::PlayerComponent>();
                if (!pl || !pos) {
                    std::cout << "can't get player" << std::endl;
                    continue;
                }
                refNetworkManager.sendToOne((size_t) netClientID,
                    System::Network::ISocket::Type::TCP,
                    playerConnection(
                        pl->getPlayerID(), pos->getX(), pos->getY()));
            }
        } else {
            std::cout << "Could not read client ID" << std::endl;
        }
    }
}

int RType::GameInstance::serverManageBuffers()
{
    auto packets = refNetworkManager.readAllPackets();
    if (packets.size() == 0)
        return 0;

    for (auto &buff : packets) {
        std::string buffer = buff;
        std::string codeStr = buffer.substr(0, 3);
        int code = atoi(codeStr.c_str());
        int code_int = is_code_valid(code);
        std::vector<std::string> tokens;
        if (code_int == -1) {
            return -1;
        }
        std::string str = buffer.substr(4, buffer.size() - 4);
        std::istringstream ss(str);
        std::string token;
        std::cout << "Managing Buffer: " << buffer << std::endl;
        while (std::getline(ss, token, ' ')) {
            tokens.push_back(token);
        }
        switch (code_int) {
            case 0: serverHandlePlayer(code, tokens); break;
            // case 1: handle_enemy(code, tokens); break;
            // case 2: handle_terrain(code, tokens); break;
            // case 3: handle_mechs(code, tokens); break;
            case 9: serverHanlderValidateConnection(code, tokens); break;

            default: break;
        }
    }
    return 0;
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
