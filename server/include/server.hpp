/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#ifndef SERVER_HPP
#define SERVER_HPP
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

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

class server {
  public:
    server();
    ~server();
    void create_server();
    void receive_message();
    int playerConnection(int id, int x, int y);
    int playerPosition(int id, int x, int y);
    int playerKilled(int id);
    int playerShooting(int id, int x, int y);
    int playerDamaged(int id, int amount);
    int playerDisconnection(int id);
    int handle_player(int code, std::vector<std::string> tokens);

  private:
    System::Network::UDPSocket _sockupd;
    std::vector<Player_t> _players;
    std::vector<Enemy_t> _enemies;
    std::vector<Terrain_t> _terrains;
};

int handle_player(int code, std::vector<std::string> tokens);
#endif /* SERVER_HPP */
