/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** players
*/

#include "server.hpp"

int server::playerConnection(int id, int x, int y)
{
    // Connect player to server
    return 0;
}

int server::playerPosition(int id, int x, int y)
{
    Player_t *player;
    for (size_t i = 0; i != _players.size(); i++) {
        if (_players[i].id == id)
            player = &_players[i];
    }
    // Update player position
    player->x = x;
    player->y = y;
    // send to all
    return 0;
}

int server::playerKilled(int id)
{
    _players.erase(_players.begin() + id);
    // send to all
    return 0;
}

int server::playerShooting(int id, int x, int y)
{
    // send to all projectiles at x y
    return 0;
}

int server::playerDamaged(int id, int amount)
{
    Player_t *player;
    for (size_t i = 0; i != _players.size(); i++) {
        if (_players[i].id == id)
            player = &_players[i];
    }

    player->hp -= amount;
    if (player->hp <= 0)
        playerKilled(id);
    // else
    //  send to all;
    return 0;
}

int server::playerDisconnection(int id)
{
    playerKilled(id);
    return 0;
}
