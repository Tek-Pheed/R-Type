/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HealthSystem
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cmath>
#include "Components.hpp"
#include "Game.hpp"
#include "GameSystems.hpp"

using namespace RType;
using namespace GameSystems;

void HealthSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

void setNewHealth(GameInstance &game, ecs::Entity &player)
{
    if (game.getHealthId() == -1)
        return;
    auto &healthEnt = game.refEntityManager.getCurrentLevel().getEntityById(
        static_cast<size_t>(game.getHealthId()));
    auto healthText = healthEnt.getComponent<ecs::TextComponent<sf::Text>>();
    auto healthPlayer = player.getComponent<ecs::HealthComponent>();

    if (healthText && healthPlayer) {
        if (floor(healthPlayer->getHealth()) <= 0) {
            healthText->setStr("Health: Dead");
        } else
            healthText->setStr(
                "Health: " + std::to_string(healthPlayer->getHealth()));
    } else if (healthText && !healthPlayer) {
        healthText->setStr("Health: Dead");
    }
}

void HealthSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    (void) deltaTime;
    for (auto &entity : entities) {
        auto health = entity.getComponent<ecs::HealthComponent>();
        if (!health)
            continue;
        if (health->getHealth() != health->getOldHealth()) {
            health->setOldHealth(health->getHealth());
            if (!_game->isServer() && _game->hasLocalPlayer()) {
                if (entity.getComponent<ecs::PlayerComponent>()
                    && entity.getComponent<ecs::PlayerComponent>()
                            ->getPlayerID()
                        == _game->getLocalPlayer()
                               .getComponent<ecs::PlayerComponent>()
                               ->getPlayerID()) {
                    setNewHealth(*_game, entity);
                }
            }
        }
        if (floor(health->getHealth()) <= 0) {
            if (entity.getComponent<ecs::PlayerComponent>()) {
                _game->deletePlayer(
                    entity.getComponent<ecs::PlayerComponent>()
                        ->getPlayerID());
            } else if (entity.getComponent<ecs::EnemyComponent>()) {
                _game->deleteEnemy(
                    entity.getComponent<ecs::EnemyComponent>()->getEnemyID());
            }
        }
    }
}
