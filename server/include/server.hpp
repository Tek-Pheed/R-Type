/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstddef>
#include <mutex>
#include <sys/types.h>
#include <vector>
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

    Client()
        : tcpSocket(System::Network::TCPSocket()), ip(std::string()),
          port(uint16_t()), readBufferTCP(std::string()),
          writeBufferTCP(std::string()), readBufferUDP(std::string()),
          writeBufferUDP(std::string()), isReady(false) {};
};

class server {
  public:
    server();
    ~server();
    void create_server();
    void createClient();

    Client &addClient(const Client &client);
    Client &getClient(size_t id);
    void removeClient(size_t id);

    void receive_message();
    void handle_connection();
    void send_to_all(std::string message);

    void handle_packet(
        size_t clientID, System::Network::ISocket::Type socketType);

    void writeToClient(Client &client, const std::string &data,
        System::Network::ISocket::Type socketType);

    void threadedServerRead();
    void threadedServerWrite();
    int playerConnection(int id, int x, int y);
    int playerPosition(int id, int x, int y);
    int playerKilled(int id);
    int playerShooting(int id, int x, int y);
    int playerDamaged(int id, int amount);
    int playerDisconnection(int id);
    int handle_player(int code, std::vector<std::string> tokens);
    int manage_buffer(std::string buffer);

    ssize_t identifyClient(const System::Network::ISocket &socket);
    ssize_t identifyClient(const std::string &ip, const std::string &port);

    ssize_t authenticateUDPClient(const System::Network::byteArray &packet);

  private:
    std::mutex _globalMutex;
    std::condition_variable _writeCondition;
    std::mutex _writeMutex;

    size_t _clientCounter;
    System::Network::TCPSocket _serverSocketTCP;
    System::Network::UDPSocket _serverSocketUDP;
    std::unordered_map<size_t, Client> _clients;

    std::vector<Player_t> _players;
    std::vector<Enemy_t> _enemies;
    std::vector<Terrain_t> _terrains;
};

int handle_player(int code, std::vector<std::string> tokens);
#endif /* SERVER_HPP */
