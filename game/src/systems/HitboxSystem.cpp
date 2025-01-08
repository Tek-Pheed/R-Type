/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** HitboxSystem.cpp
*/

#include "Components.hpp"
#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cmath>
#include "Game.hpp"
#include "GameSystems.hpp"

using namespace RType;
using namespace GameSystems;

void HitboxSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

void HitboxSystem::EnemyCollision(ecs::Entity &enemy, float deltaTime)
{
    auto enemyPos = enemy.getComponent<ecs::PositionComponent>();
    auto enemyComp = enemy.getComponent<ecs::EnemyComponent>();
    auto enemyHitB = enemy.getComponent<ecs::HitboxComponent>();
    const float hitbox = 5.0f;

    if (!enemyPos || !enemyComp || !enemyHitB)
        return;

    static float damageCooldown = 0.0f;
    damageCooldown -= deltaTime;

    for (auto &entity :
        _game->refEntityManager.getCurrentLevel().getEntities()) {
        auto enti = entity.get();

        if (enti.getID() == enemyComp->getEnemyID())
            continue;

        auto position = enti.getComponent<ecs::PositionComponent>();
        auto player = enti.getComponent<ecs::PlayerComponent>();
        auto health = enti.getComponent<ecs::HealthComponent>();
        auto playerHitbox = enti.getComponent<ecs::HitboxComponent>();

        if (!position || !player || !playerHitbox)
            continue;

        float playerCenterX = position->getX() + playerHitbox->getWidth() / 2;
        float playerCenterY = position->getY() + playerHitbox->getHeight() / 2;

        if (enemyPos->getX() + enemyHitB->getWidth() / 2
                < playerCenterX + playerHitbox->getWidth() / 2 + hitbox
            && enemyPos->getX() + enemyHitB->getWidth() / 2
                > playerCenterX - playerHitbox->getWidth() / 2 - hitbox
            && enemyPos->getY() + enemyHitB->getHeight() / 2
                < playerCenterY + playerHitbox->getHeight() / 2 + hitbox
            && enemyPos->getY() + enemyHitB->getHeight() / 2
                > playerCenterY - playerHitbox->getHeight() / 2 - hitbox) {
            if (_game->isServer() && damageCooldown <= 0.0f) {
                _game->damagePlayer(player->getPlayerID(), 50);
                _game->deleteEnemy(enemyComp->getEnemyID());
            }

            if (!_game->isServer()
                && player->getPlayerID()
                    == _game->getLocalPlayer()
                           .getComponent<ecs::PlayerComponent>()
                           ->getPlayerID()
                && damageCooldown <= 0.0f) {
                auto healthEnt =
                    _game->refEntityManager.getCurrentLevel().getEntityById(
                        _game->getHealthId());
                auto healthText =
                    healthEnt.getComponent<ecs::TextComponent<sf::Text>>();
                if (healthText && health) {
                    if (floor(health->getHealth() - 50) <= 0) {
                        healthText->setStr("Health: Dead");
                    } else
                        healthText->setStr("Health: "
                            + std::to_string(health->getHealth() - 50));
                } else if (healthText && !health) {
                    healthText->setStr("Health: Dead");
                }
            }
            damageCooldown = 1.0f;
        }
    }
}

void HitboxSystem::BulletCollision(ecs::Entity &bullet)
{
    auto bulletPos = bullet.getComponent<ecs::PositionComponent>();
    auto bulletComp = bullet.getComponent<ecs::BulletComponent>();
    auto bulletHitB = bullet.getComponent<ecs::HitboxComponent>();

    if (!bulletPos || !bulletComp || !bulletHitB)
        return;

    for (auto &entity :
        _game->refEntityManager.getCurrentLevel().getEntities()) {
        auto enti = entity.get();
        auto position = enti.getComponent<ecs::PositionComponent>();
        auto enemy = enti.getComponent<ecs::EnemyComponent>();
        auto health = enti.getComponent<ecs::HealthComponent>();
        auto enemyHitB = enti.getComponent<ecs::HitboxComponent>();
        if (!position || !enemy || !health || !enemyHitB)
            continue;
        if (enti.getID() == bullet.getID())
            continue;

        float enemyCenterX = position->getX() + enemyHitB->getWidth() / 2.0f;
        float enemyCenterY = (position->getY() + enemyHitB->getHeight() / 2.0f)
            + enemyHitB->getHeight() / 2.0f;

        float bulletCenterX =
            bulletPos->getX() + bulletHitB->getWidth() / 2.0f;
        float bulletCenterY =
            bulletPos->getY() + bulletHitB->getHeight() / 2.0f;

        float enemyHalfWidth = enemyHitB->getWidth() / 2;
        float enemyHalfHeight = enemyHitB->getHeight() / 2;

        float bulletHalfWidth = bulletHitB->getWidth() / 2;
        float bulletHalfHeight = bulletHitB->getHeight() / 2;

        if (std::abs(bulletCenterX - enemyCenterX)
                < (enemyHalfWidth + bulletHalfWidth)
            && std::abs(bulletCenterY - enemyCenterY)
                < (enemyHalfHeight + bulletHalfHeight)) {
            if (_game->isServer()) {
                health->setHealth(health->getHealth() - 100);
            }
            _game->refEntityManager.getCurrentLevel().destroyEntityById(
                bullet.getID());
        }
    }
}

void HitboxSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    for (auto &entity : entities) {
        auto bullet = entity.getComponent<ecs::BulletComponent>();
        auto enemy = entity.getComponent<ecs::EnemyComponent>();

        auto hitbox = entity.getComponent<ecs::HitboxComponent>();
        auto position = entity.getComponent<ecs::PositionComponent>();
        auto velocity = entity.getComponent<ecs::VelocityComponent>();

        if (!hitbox)
            continue;

        if (bullet && position && velocity && hitbox) {
            BulletCollision(entity);
        }
        if (enemy && position && velocity && hitbox) {
            EnemyCollision(entity, deltaTime);
        }
    }
}
