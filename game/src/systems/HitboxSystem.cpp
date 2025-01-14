/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** HitboxSystem.cpp
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

    for (size_t id :
        _game->refEntityManager.getCurrentLevel()
            .findEntitiesIdByComponent<ecs::PositionComponent>()) {
        try {
            auto &enti =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);

            if (enti.getID() == enemyComp->getEnemyID())
                continue;

            auto position = enti.getComponent<ecs::PositionComponent>();
            auto player = enti.getComponent<ecs::PlayerComponent>();
            auto health = enti.getComponent<ecs::HealthComponent>();
            auto playerHitbox = enti.getComponent<ecs::HitboxComponent>();

            if (!position || !player || !playerHitbox)
                continue;

            float playerCenterX =
                position->getX() + playerHitbox->getWidth() / 2;
            float playerCenterY =
                position->getY() + playerHitbox->getHeight() / 2;

            if (enemyPos->getX() + enemyHitB->getWidth() / 2
                    < playerCenterX + playerHitbox->getWidth() / 2 + hitbox
                && enemyPos->getX() + enemyHitB->getWidth() / 2
                    > playerCenterX - playerHitbox->getWidth() / 2 - hitbox
                && enemyPos->getY() + enemyHitB->getHeight() / 2
                    < playerCenterY + playerHitbox->getHeight() / 2 + hitbox
                && enemyPos->getY() + enemyHitB->getHeight() / 2
                    > playerCenterY - playerHitbox->getHeight() / 2 - hitbox) {
                if (_game->isServer() && damageCooldown <= 0.0f) {
                    if (enemyComp->getType() == 0
                        || enemyComp->getType() == 1) {
                        _game->damagePlayer(player->getPlayerID(), 50);
                        _game->deleteEnemy(enemyComp->getEnemyID());
                    }
                    if (enemyComp->getType() == 2) {
                        _game->damagePlayer(player->getPlayerID(), 100);
                    }
                    damageCooldown = 1.0f;
                }
            }
        } catch (const std::exception &e) {
            std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::PlayerBulletCollision(ecs::Entity &bullet)
{
    auto bulletPos = bullet.getComponent<ecs::PositionComponent>();
    auto bulletComp = bullet.getComponent<ecs::BulletComponent>();
    auto bulletHitB = bullet.getComponent<ecs::HitboxComponent>();

    if (!bulletPos || !bulletComp || !bulletHitB)
        return;

    for (size_t id :
        _game->refEntityManager.getCurrentLevel()
            .findEntitiesIdByComponent<ecs::PositionComponent>()) {
        try {
            auto &enti =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);
            auto position = enti.getComponent<ecs::PositionComponent>();
            auto enemy = enti.getComponent<ecs::EnemyComponent>();
            auto health = enti.getComponent<ecs::HealthComponent>();
            auto enemyHitB = enti.getComponent<ecs::HitboxComponent>();
            if (!position || !enemy || !health || !enemyHitB)
                continue;
            if (enti.getID() == bullet.getID())
                continue;

            float enemyCenterX =
                position->getX() + enemyHitB->getWidth() / 2.0f;
            float enemyCenterY =
                position->getY() + enemyHitB->getHeight() / 2.0f;

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
                _game->refEntityManager.getCurrentLevel()
                    .markEntityForDeletion(bullet.getID());
            }
        } catch (const std::exception &e) {
            std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::EnemyBulletCollision(ecs::Entity &bullet)
{
    auto bulletPos = bullet.getComponent<ecs::PositionComponent>();
    auto bulletComp = bullet.getComponent<ecs::BulletComponent>();
    auto bulletHitB = bullet.getComponent<ecs::HitboxComponent>();
    const int DMG = 20;

    if (!bulletPos || !bulletComp || !bulletHitB)
        return;

    for (size_t id :
        _game->refEntityManager.getCurrentLevel()
            .findEntitiesIdByComponent<ecs::PositionComponent>()) {
        try {
            auto &enti =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);
            auto position = enti.getComponent<ecs::PositionComponent>();
            auto player = enti.getComponent<ecs::PlayerComponent>();
            auto health = enti.getComponent<ecs::HealthComponent>();
            auto playerHitb = enti.getComponent<ecs::HitboxComponent>();
            if (!position || !player || !health || !playerHitb)
                continue;
            if (enti.getID() == bullet.getID())
                continue;

            float enemyCenterX =
                position->getX() + playerHitb->getWidth() / 2.0f;
            float enemyCenterY =
                position->getY() + playerHitb->getHeight() / 2.0f;

            float bulletCenterX =
                bulletPos->getX() + bulletHitB->getWidth() / 2.0f;
            float bulletCenterY =
                bulletPos->getY() + bulletHitB->getHeight() / 2.0f;

            float enemyHalfWidth = playerHitb->getWidth() / 2;
            float enemyHalfHeight = playerHitb->getHeight() / 2;

            float bulletHalfWidth = bulletHitB->getWidth() / 2;
            float bulletHalfHeight = bulletHitB->getHeight() / 2;

            if (std::abs(bulletCenterX - enemyCenterX)
                    < (enemyHalfWidth + bulletHalfWidth)
                && std::abs(bulletCenterY - enemyCenterY)
                    < (enemyHalfHeight + bulletHalfHeight)) {
                if (_game->isServer()) {
                    if (bulletComp->getType() == 0)
                        _game->damagePlayer(player->getPlayerID(), DMG);
                    if (bulletComp->getType() == 1)
                        _game->damagePlayer(player->getPlayerID(), DMG * 3);
                }
                _game->refEntityManager.getCurrentLevel()
                    .markEntityForDeletion(bullet.getID());
            }
        } catch (const std::exception &e) {
            std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::BonusCollision(ecs::Entity &bonus)
{
    auto bonusPos = bonus.getComponent<ecs::PositionComponent>();
    auto bonusComp = bonus.getComponent<ecs::BonusComponent>();
    auto bonusHitB = bonus.getComponent<ecs::HitboxComponent>();

    if (!bonusPos || !bonusComp || !bonusHitB)
        return;

    for (size_t id : _game->refEntityManager.getCurrentLevel()
             .findEntitiesIdByComponent<ecs::PositionComponent>()) {
        try {
            auto &enti =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);
            auto position = enti.getComponent<ecs::PositionComponent>();
            auto player = enti.getComponent<ecs::PlayerComponent>();
            auto playerHitB = enti.getComponent<ecs::HitboxComponent>();
            if (!position || !player || !playerHitB)
                continue;
            if (enti.getID() == bonus.getID())
                continue;

            float enemyCenterX =
                position->getX() + playerHitB->getWidth() / 2.0f;
            float enemyCenterY =
                position->getY() + playerHitB->getHeight() / 2.0f;

            float bulletCenterX =
                bonusPos->getX() + bonusHitB->getWidth() / 2.0f;
            float bulletCenterY =
                bonusPos->getY() + bonusHitB->getHeight() / 2.0f;

            float playerHalfWidth = playerHitB->getWidth() / 2;
            float playerHalfHeight = playerHitB->getHeight() / 2;

            float bulletHalfWidth = bonusHitB->getWidth() / 2;
            float bulletHalfHeight = bonusHitB->getHeight() / 2;

            if (std::abs(bulletCenterX - enemyCenterX)
                    < (playerHalfWidth + bulletHalfWidth)
                && std::abs(bulletCenterY - enemyCenterY)
                    < (playerHalfHeight + bulletHalfHeight)) {
                _game->refEntityManager.getCurrentLevel()
                    .markEntityForDeletion(bonus.getID());
            }
        } catch (const std::exception &e) {
            std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    (void) entities;
    for (size_t id :
        _game->refEntityManager.getCurrentLevel()
            .findEntitiesIdByComponent<ecs::PositionComponent>()) {
        try {
            auto &entity =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);
            auto bullet = entity.getComponent<ecs::BulletComponent>();
            auto enemy = entity.getComponent<ecs::EnemyComponent>();
            auto bonus = entity.getComponent<ecs::BonusComponent>();

            auto hitbox = entity.getComponent<ecs::HitboxComponent>();
            auto position = entity.getComponent<ecs::PositionComponent>();
            auto velocity = entity.getComponent<ecs::VelocityComponent>();

            if (!hitbox)
                continue;

            if (bullet && position && velocity && hitbox
                && bullet->getIsFromPlayer() == true) {
                PlayerBulletCollision(entity);
            }
            if (bullet && position && velocity && hitbox
                && bullet->getIsFromPlayer() == false) {
                EnemyBulletCollision(entity);
            }
            if (_game->isServer() && enemy && position && velocity && hitbox) {
                EnemyCollision(entity, deltaTime);
            }
            if (bonus && position && velocity && hitbox) {
                BonusCollision(entity);
            }
        } catch (const std::exception &e) {
            std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}
