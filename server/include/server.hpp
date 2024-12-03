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
/*struct server {
    int scokfd;
    struct sockaddr_in servaddr, cliaddr;
};*/

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

  private:
    int _sockfd;
    struct sockaddr_in _servaddr, _cliaddr;
    std::vector<Player_t> _players;
    std::vector<Enemy_t> _enemies;
    std::vector<Terrain_t> _terrains;
};

int handle_player(int code, std::vector<std::string> tokens);
#endif /* SERVER_HPP */
