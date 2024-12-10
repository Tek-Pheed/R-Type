/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cstddef>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <vector>
#include "Entity.hpp"
#include "protocol.hpp"
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"
#include <condition_variable>
#include <unordered_map>

struct Player_s {
    int id;
    int x;
    int y;
    int hp;
};
typedef struct Player_s Player_t;

struct Enemy_s {
    int id;
    int x;
    int y;
    int hp;
};
typedef struct Enemy_s Enemy_t;

struct Terrain_s {
    int id;
    int x;
    int y;
    int hp;
};
typedef struct Terrain_s Terrain_t;

class Client {
  public:
    System::Network::TCPSocket tcpSocket;
    std::string ip;
    uint16_t port;
    std::string readBufferTCP;
    std::string writeBufferTCP;
    std::string readBufferUDP;
    std::string writeBufferUDP;
    bool isReady;
    bool isDisconnected;

    Client()
        : tcpSocket(System::Network::TCPSocket()), ip(std::string()),
          port(uint16_t()), readBufferTCP(std::string()),
          writeBufferTCP(std::string()), readBufferUDP(std::string()),
          writeBufferUDP(std::string()), isReady(false),
          isDisconnected(false) {};
};

template <typename T> std::string getString(T arg)
{
    return (std::to_string(arg));
}

template <> inline std::string getString(const char *arg)
{
    return (std::string(arg));
}

template <> inline std::string getString(const std::string &arg)
{
    return (arg);
}

template <> inline std::string getString(std::string &arg)
{
    return (arg);
}

template <> inline std::string getString(std::string arg)
{
    return (arg);
}

class server {
  public:
    static constexpr int MAX_PLAYERS = 4;

    server();
    ~server();

    template <typename... Args>
    std::string makePacket(int protocolCode, Args... args)
    {
        std::string pack = std::to_string(protocolCode);

        for (const auto &ag : {args...}) {
            pack += " " + getString(ag);
        }
        pack += PACKET_END;
        return (pack);
    }

    void create_server();
    void createClient();

    Client &addClient(const Client &client);
    Client &getClient(size_t id);
    void removeClient(size_t id);

    void receive_message();
    void handle_connection();
    void send_to_all(const std::string &message);
    void send_to_others(const std::string &message, size_t except_clientID);
    void send_to_one(const std::string &message, size_t clientID);

    void handle_packet(
        size_t clientID, System::Network::ISocket::Type socketType);

    void writeToClient(Client &client, const std::string &data,
        System::Network::ISocket::Type socketType);

    void threadedServerRead();
    void threadedServerWrite();

    template <typename T>
    std::vector<std::shared_ptr<ecs::Entity>> getEntitiesByComponent()
    {
        std::vector<std::shared_ptr<ecs::Entity>> ent;
        std::shared_ptr<T> comp = nullptr;

        for (auto entity : _gameState) {
            comp = entity->getComponent<T>();
            if (comp != nullptr)
                ent.emplace_back(entity);
        }
        return (ent);
    }

    template <typename T> int findEntityByComponent()
    {
        std::shared_ptr<T> comp = nullptr;
        size_t i = 0;

        for (auto entity : _gameState) {
            comp = entity->getComponent<T>();
            if (comp != nullptr)
                return ((int) i);
            i++;
        }
        return (-1);
    }

    void syncNewClientGameState(size_t newClient);

    std::shared_ptr<ecs::Entity> getPlayer(size_t playerID);
    int playerConnection(size_t id);
    int playerPosition(int id, float x, float y);
    int playerKilled(size_t id);
    int playerShooting(int id, int x, int y);
    int playerDamaged(int id, int amount);
    int playerDisconnection(size_t id);
    int handle_player(int code, const std::vector<std::string> &tokens);
    int manage_buffer(std::string buffer);

    ssize_t identifyClient(const System::Network::ISocket &socket);
    ssize_t identifyClient(const std::string &ip, const std::string &port);

    ssize_t authenticateUDPClient(const System::Network::byteArray &packet);

    void handleNewPlayer(size_t id);

  private:
    std::mutex _globalMutex;
    std::condition_variable _writeCondition;
    std::mutex _writeMutex;

    size_t _clientCounter;
    System::Network::TCPSocket _serverSocketTCP;
    System::Network::UDPSocket _serverSocketUDP;

    std::unordered_map<size_t, Client> _clients;
    std::vector<std::shared_ptr<ecs::Entity>> _gameState;

    std::vector<Enemy_t> _enemies;
    std::vector<Terrain_t> _terrains;

    int _currentPlayers;
};

int handle_player(int code, std::vector<std::string> tokens);
#endif /* SERVER_HPP */
