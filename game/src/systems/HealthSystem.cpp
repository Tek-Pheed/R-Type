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

void HealthSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    (void) deltaTime;
    for (auto &entity : entities) {
        auto health = entity.getComponent<ecs::HealthComponent>();
        if (!health)
            continue;
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
