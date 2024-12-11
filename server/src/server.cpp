/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "server.hpp"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "protocol.hpp"
#include "server.hpp"

#define TCP_PORT 8081
#define UDP_PORT 8082

static int is_code_valid(int code)
{
    if (code >= P_CONN && code <= P_DISCONN)
        return 0;
    if (code >= E_SPAWN && code <= E_DMG)
        return 1;
    if (code >= T_SPAWN && code <= T_DEAD)
        return 2;
    if (code >= M_WAVE && code <= M_GOVER)
        return 3;
    if (code >= C_INIT_UDP && code <= C_START_UDP)
        return 9;
    return -1;
}

server::server()
    : _clientCounter(0), _serverSocketTCP(System::Network::TCPSocket()),
      _enemyTimerMS(0), _currentPlayers(0)
{
    return;
}

server::~server()
{
}

void server::writeToClient(Client &client, const std::string &data,
    System::Network::ISocket::Type socketType)
{
    std::unique_lock lock(_writeMutex);

    if (socketType == System::Network::ISocket::TCP) {
        client.writeBufferTCP += data;
    }
    if (socketType == System::Network::ISocket::UDP) {
        client.writeBufferUDP += data;
    }
    _writeCondition.notify_one();
}

void server::handleNewPlayer(size_t id)
{
    auto player = std::make_shared<ecs::Entity>(id);
    player->addComponent(std::make_shared<ecs::PlayerComponent>(
        std::string("Player_") + std::to_string(id)));
    player->addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    _gameState.emplace_back(player);
    playerConnection(id);
}

void server::handle_packet(
    size_t clientID, System::Network::ISocket::Type socketType)
{
    Client &client = getClient(clientID);
    std::unique_lock lock(_globalMutex);

    if (socketType == System::Network::ISocket::TCP) {
        client.readBufferTCP.clear();
    }
    if (socketType == System::Network::ISocket::UDP) {
        if (!client.isReady) {
            std::string accept = "";
            if (_clients.size() > MAX_PLAYERS) {
                accept = makePacket(C_AUTH, "KO");
                std::cout
                    << "Server: Maximum number of player reached, cannot "
                       "accept new player curently."
                    << std::endl;
            } else {
                accept = makePacket(C_AUTH, "OK");
                client.isReady = true;
                handleNewPlayer(clientID);
                syncNewClientGameState(clientID);
            }
            this->writeToClient(client, accept, System::Network::ISocket::TCP);
        } else {
            if (client.readBufferUDP.length() != 0) {
                std::string buffer = client.readBufferUDP;
                std::string codeStr = std::string(buffer).substr(0, 3);
                int code = atoi(codeStr.c_str());
                int code_int = is_code_valid(code);
                std::vector<std::string> tokens;
                if (code_int == -1) {
                    std::cout << "Server: Invalid Protocol Code: "
                              << std::to_string(code_int) << std::endl;
                    client.readBufferUDP.clear();
                    return;
                }
                std::string str = buffer.substr(4, buffer.size() - 4);
                std::istringstream ss(str);
                std::string token;
                while (std::getline(ss, token, ' ')) {
                    tokens.push_back(token);
                }
                if (code >= P_SETUP && code <= P_DISCONN) {
                    handle_player(code, tokens);
                }
            }
            client.readBufferUDP.clear();
        }
    }
}

void server::handleConnection()
{
    System::Network::socketSetGeneric readfds;

    while (true) {
        readfds.clear();
        readfds.emplace_back(&_serverSocketTCP);
        try {
            System::Network::select(&readfds);
        } catch (const std::exception &s) {
            std::cerr << "An error occured on the server: " << s.what()
                      << std::endl;
        }
        if (readfds.size() == 0)
            continue;
        Client cl;
        cl.tcpSocket = System::Network::accept(_serverSocketTCP);
        Client &client = addClient(cl);
        std::string toWrite = makePacket(C_INIT_UDP, _clientCounter);
        writeToClient(client, toWrite, System::Network::ISocket::TCP);
    }
}

void server::create_server()
{
    _serverSocketTCP.initSocket(TCP_PORT, System::Network::ISocket::SERVE);
    _serverSocketUDP.initSocket(UDP_PORT);
    std::cout << "Running server on port TCP:" << std::to_string(TCP_PORT)
              << ", UDP:" << std::to_string(UDP_PORT) << std::endl;
}

int server::manage_buffer(std::string buffer)
{
    std::string code = std::string(buffer).substr(0, 3);
    int code_int = is_code_valid(atoi(code.c_str()));
    std::vector<std::string> tokens;

    if (code_int == -1)
        return -1;
    std::string str = buffer.substr(4, buffer.size() - 4);
    std::istringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    switch (code_int) {
        case 0:
            for (size_t i = 0; i < tokens.size(); i++) {
                printf("args: %s\n", tokens[i].c_str());
            }
            handle_player(atoi(code.c_str()), tokens);
            break;
        case 1:
            // handle_enemy(tokens);
            break;
        case 2:
            // handle_terrain(tokens);
            break;
        case 3:
            // handle_mechs(tokens);
            break;
        case 9:
            // handle_connection(tokens);
            break;
        default: break;
    }
    return 0;
}

int main()
{
    System::Network::initNetwork();
    server s;

    try {
        s.create_server();
    } catch (const std::exception &e) {
        std::cerr << "Failed to launch the server: " << e.what() << std::endl;
        return (84);
    }
    std::thread(&server::threadedServerRead, &s).detach();
    std::thread(&server::threadedServerWrite, &s).detach();
    std::thread(&server::handleConnection, &s).detach();

    auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now());
    auto last = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now());

    while (true) {
        now = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now());
        auto next = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now());
        next += std::chrono::milliseconds(1000 / SERVER_TPS);
        auto deltaTimeMs =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - last)
                .count();
        s.internalUpdate(deltaTimeMs);
        last = now;
        std::this_thread::sleep_until(next);
    }

    return (0);
}
