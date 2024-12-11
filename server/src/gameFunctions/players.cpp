/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** players
*/

#include <algorithm>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include "Components.hpp"
#include "protocol.hpp"
#include "server.hpp"

void server::syncNewClientGameState(size_t newClient)
{
    auto players = getEntitiesByComponent<ecs::PlayerComponent>();
    auto enemies = getEntitiesByComponent<ecs::EnemyComponent>();

    for (const auto &p : players) {
        if (p != nullptr) {
            auto comp_player = p->getComponent<ecs::PlayerComponent>();
            auto comp_pos = p->getComponent<ecs::PositionComponent>();
            std::string pack = "";
            if (comp_player == nullptr || comp_pos == nullptr)
                continue;
            if (p->getID() == newClient)
                continue;
            std::string id = getString(p->getID());
            std::string x = getString(comp_pos->getX());
            std::string y = getString(comp_pos->getY());
            pack = makePacket(P_CONN, id, comp_player->getName(), x, y);
            send_to_one(pack, newClient);
        }
    }
    for (const auto &p : enemies) {
        auto comp_pos = p->getComponent<ecs::PositionComponent>();
        std::string pack = "";
        if (comp_pos == nullptr)
            continue;
        std::string id = getString(p->getID());
        std::string x = getString(comp_pos->getX());
        std::string y = getString(comp_pos->getY());
        pack = makePacket(E_SPAWN, id, x, y);
        send_to_one(pack, newClient);
    }
}

std::shared_ptr<ecs::Entity> server::getPlayer(size_t playerID)
{
    auto players = getEntitiesByComponent<ecs::PlayerComponent>();

    for (const auto &pl : players) {
        if (pl != nullptr) {
            if (pl != nullptr && pl->getID() == playerID)
                return (pl);
        }
    }
    return (nullptr);
}

int server::playerConnection(size_t id)
{
    auto player = getPlayer(id);

    if (player == nullptr)
        return (-1);
    auto playerPosition = player->getComponent<ecs::PositionComponent>();
    auto playerComp = player->getComponent<ecs::PlayerComponent>();
    if (playerPosition == nullptr)
        return (-1);
    std::string idStr = std::to_string(id);
    std::string name = playerComp->getName();
    std::string xStr = std::to_string(playerPosition->getX());
    std::string yStr = std::to_string(playerPosition->getY());
    send_to_others(makePacket(P_CONN, idStr, name, xStr, yStr), id);
    return (0);
}

int server::playerPosition(int id, float x, float y)
{
    std::shared_ptr<ecs::Entity> player = getPlayer((size_t) id);

    if (player == nullptr) {
        std::cout << "Failed to update player: " << std::to_string(id)
                  << std::endl;
        return (-1);
    }
    auto pos = player->getComponent<ecs::PositionComponent>();
    if (pos == nullptr) {
        std::cout << "Failed to update player: " << std::to_string(id)
                  << std::endl;
        return (-1);
    }
    pos->setX(x);
    pos->setY(y);
    std::string pack = makePacket(player::P_POS, (float) id, x, y);
    send_to_others(pack, (size_t) id);
    return 0;
}

int server::playerKilled(size_t id)
{
    std::unique_lock lock(_globalMutex);
    auto player = getPlayer(id);

    auto foundPlayerIt =
        std::find(_gameState.begin(), _gameState.end(), player);

    if (foundPlayerIt != _gameState.end()) {
        _gameState.erase(foundPlayerIt);
    }
    std::string pack = makePacket(player::P_DEAD, id);
    send_to_all(pack);
    return (0);
}

int server::playerShooting(int id)
{
    // send to all projectiles at x y
    std::string pack = makePacket(player::P_SHOOT, id);
    send_to_others(pack, (size_t) id);
    return 0;
}

int server::playerDamaged(int id, int amount)
{
    auto playerEntity = getPlayer((size_t) id);

    if (playerEntity != nullptr) {
        auto playerHealth = playerEntity->getComponent<ecs::HealthComponent>();
        if (playerHealth != nullptr) {
            playerHealth->setHealth(playerHealth->getHealth() - amount);
            if (playerHealth->getHealth() <= 0) {
                playerKilled((size_t) id);
                return (0);
            }
            std::string pack =
                makePacket(player::P_DMG, id, playerHealth->getHealth());
            send_to_all(pack);
            return (0);
        }
        return (-1);
    }
    return (-1);
}

int server::playerDisconnection(size_t id)
{
    playerKilled(id);
    std::string pack = makePacket(player::P_DISCONN, id);
    send_to_all(pack);
    return 0;
}
