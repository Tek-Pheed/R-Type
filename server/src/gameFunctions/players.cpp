/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** players
*/

#include <cstddef>
#include "Components.hpp"
#include "server.hpp"

void server::syncNewClientGameState(size_t newClient) {
    std::cout << "The player will receive the gameState now" << std::endl;
    //TODO: Sync newly connected client
}

std::shared_ptr<ecs::Entity> server::getPlayer(size_t playerID)
{
    return (getEntityByComponent<ecs::PlayerComponent>(playerID));
}

int server::playerConnection(size_t id)
{
    auto player = getPlayer(id);

    if (player == nullptr)
        return (-1);
    auto playerPosition = player->getComponent<ecs::PositionComponent>();
    if (playerPosition == nullptr)
        return (-1);
    send_to_others(
        makePacket(P_CONN, playerPosition->getX(), playerPosition->getY()),
        id);
    return 0;
}

int server::playerPosition(int id, int x, int y)
{
    Player_t *player = nullptr;
    for (size_t i = 0; i != _players.size(); i++) {
        if (_players[i].id == id)
            player = &_players[i];
    }
    if (player == nullptr)
        return (-1);
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
    Player_t *player = nullptr;
    for (size_t i = 0; i != _players.size(); i++) {
        if (_players[i].id == id)
            player = &_players[i];
    }
    if (player == nullptr)
        return (-1);
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
