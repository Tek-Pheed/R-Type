/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <bits/stdc++.h>

#include <cstddef>
#include <mutex>
#include <vector>
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"
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

struct Client_t {
  public:
    System::Network::TCPSocket tcpSocket;
    std::string ip;
    uint16_t port;
    std::string readBuffer;
    std::string writeBuffer;

    Client_t()
        : tcpSocket(System::Network::TCPSocket()), ip(std::string()),
          port(uint16_t()), readBuffer(std::string()),
          writeBuffer(std::string()){};
};

class server {
  public:
    server();
    ~server();
    void create_server();
    void createClient();

    void addClient(Client_t &client);
    Client_t &getClient(size_t id);
    void removeClient(size_t id);

    void receive_message();
    void handle_connection();
    void send_to_all(std::string message);

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

    size_t identifyClient(System::Network::TCPSocket socket);
    size_t identifyClient(std::string ip, std::string port);

  private:
    size_t _clientCounter;
    System::Network::TCPSocket _serverSocketTCP;
    System::Network::UDPSocket _serverSocketUDP;
    std::mutex _mutex;
    std::unordered_map<size_t, Client_t> _clients;

    std::vector<Player_t> _players;
    std::vector<Enemy_t> _enemies;
    std::vector<Terrain_t> _terrains;
};

int handle_player(int code, std::vector<std::string> tokens);
#endif /* SERVER_HPP */
