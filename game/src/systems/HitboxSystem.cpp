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
#include <sstream>
#include "Components.hpp"
#include "Game.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"

using namespace RType;
using namespace GameSystems;

void HitboxSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

static bool isInBounds(float firstCenterX, float firstCenterY,
    float firstWidth, float firstHeight, float secondCenterX,
    float secondCenterY, float secondWidth, float secondHeight)
{
    float firstLeft = firstCenterX - (firstWidth / 2.0f);
    float firstRight = firstCenterX + (firstWidth / 2.0f);
    float firstTop = firstCenterY - (firstHeight / 2.0f);
    float firstBottom = firstCenterY + (firstHeight / 2.0f);
    float secondLeft = secondCenterX - (secondWidth / 2.0f);
    float secondRight = secondCenterX + (secondWidth / 2.0f);
    float secondTop = secondCenterY - (secondHeight / 2.0f);
    float secondBottom = secondCenterY + (secondHeight / 2.0f);

    if (firstRight < secondLeft || firstLeft > secondRight
        || firstBottom < secondTop || firstTop > secondBottom)
        return (false);
    return (true);
}

void HitboxSystem::EnemyCollision(ecs::Entity &enemy, float deltaTime)
{
    auto enemyPos = enemy.getComponent<ecs::PositionComponent>();
    auto enemyComp = enemy.getComponent<ecs::EnemyComponent>();
    auto enemyHitB = enemy.getComponent<ecs::HitboxComponent>();
    const int E_DMG = 50 * static_cast<int>(_game->getDifficulty());
    const int B_DMG = 100 * static_cast<int>(_game->getDifficulty());

    if (!enemyPos || !enemyComp || !enemyHitB)
        return;

    static float damageCooldown = 0.0f;
    damageCooldown -= deltaTime;

    for (size_t id : _game->refEntityManager.getCurrentLevel()
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

            if (isInBounds(position->getX(), position->getY(),
                    playerHitbox->getWidth(), playerHitbox->getHeight(),
                    enemyPos->getX(), enemyPos->getY(), enemyHitB->getWidth(),
                    enemyHitB->getHeight())) {
                if (_game->isServer() && damageCooldown <= 0.0f
                    && enemyComp->getWave() == _game->currentWave) {
                    if (enemyComp->getType() == 0
                        || enemyComp->getType() == 1) {
                        _game->damagePlayer(player->getPlayerID(), E_DMG);
                        _game->deleteEnemy(enemyComp->getEnemyID());
                    }
                    if (enemyComp->getType() == 2) {
                        _game->damagePlayer(player->getPlayerID(), B_DMG);
                    }
                    damageCooldown = 1.0f;
                }
            }
        } catch (const std::exception &e) {
            if constexpr (RType::GameInstance::DEBUG_LOGS)
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

    for (size_t id : _game->refEntityManager.getCurrentLevel()
             .findEntitiesIdByComponent<ecs::EnemyComponent>()) {
        try {
            auto &enti =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);
            auto position = enti.getComponent<ecs::PositionComponent>();
            auto enemy = enti.getComponent<ecs::EnemyComponent>();
            auto health = enti.getComponent<ecs::HealthComponent>();
            auto enemyHitB = enti.getComponent<ecs::HitboxComponent>();
            if (!position || !enemy || !health || !enemyHitB)
                continue;
            if (enti.getID() == bullet.getID()
                || enemy->getWave() != _game->currentWave)
                continue;
            if (isInBounds(position->getX(), position->getY(),
                    enemyHitB->getWidth(), enemyHitB->getHeight(),
                    bulletPos->getX(), bulletPos->getY(),
                    bulletHitB->getWidth(), bulletHitB->getHeight())) {
                if constexpr (server) {
                    health->setHealth(health->getHealth() - 100);
                }
                _game->refEntityManager.getCurrentLevel()
                    .markEntityForDeletion(bullet.getID());
            }
        } catch (const std::exception &e) {
            if constexpr (RType::GameInstance::DEBUG_LOGS)
                std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::EnemyBulletCollision(ecs::Entity &bullet)
{
    auto bulletPos = bullet.getComponent<ecs::PositionComponent>();
    auto bulletComp = bullet.getComponent<ecs::BulletComponent>();
    auto bulletHitB = bullet.getComponent<ecs::HitboxComponent>();
    const int DMG = 20 * static_cast<int>(_game->getDifficulty());

    if (!bulletPos || !bulletComp || !bulletHitB)
        return;

    for (size_t id : _game->refEntityManager.getCurrentLevel()
             .findEntitiesIdByComponent<ecs::PlayerComponent>()) {
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
            if (isInBounds(position->getX(), position->getY(),
                    playerHitb->getWidth(), playerHitb->getHeight(),
                    bulletPos->getX(), bulletPos->getY(),
                    bulletHitB->getWidth(), bulletHitB->getHeight())) {
                if constexpr (server) {
                    if (bulletComp->getType() == 0)
                        _game->damagePlayer(player->getPlayerID(), DMG);
                    if (bulletComp->getType() == 1)
                        _game->damagePlayer(player->getPlayerID(), DMG * 3);
                }
                _game->refEntityManager.getCurrentLevel()
                    .markEntityForDeletion(bullet.getID());
            }
        } catch (const std::exception &e) {
            if constexpr (RType::GameInstance::DEBUG_LOGS)
                std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::BonusCollision(ecs::Entity &bonus)
{
    auto bonusPos = bonus.getComponent<ecs::PositionComponent>();
    auto bonusComp = bonus.getComponent<ecs::BonusComponent>();
    auto bonusHitB = bonus.getComponent<ecs::HitboxComponent>();
    auto playerComp = bonus.getComponent<ecs::PlayerComponent>();

    if (!bonusPos || !bonusComp || !bonusHitB || playerComp)
        return;

    for (size_t id : _game->refEntityManager.getCurrentLevel()
             .findEntitiesIdByComponent<ecs::PlayerComponent>()) {
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
            if (isInBounds(position->getX(), position->getY(),
                    playerHitB->getWidth(), playerHitB->getHeight(),
                    bonusPos->getX(), bonusPos->getY(), bonusHitB->getWidth(),
                    bonusHitB->getHeight())) {
                //_game->applyBonus(bonus, player->getPlayerID());
                std::stringstream sss;
                sss << BN_GET << " " << player->getPlayerID() << " "
                    << bonusComp->getBonusID() << " " << bonusComp->getBonus()
                    << " " << PACKET_END;
                _game->refNetworkManager.sendToAll(
                    System::Network::ISocket::Type::TCP, sss.str());
            }
        } catch (const std::exception &e) {
            if constexpr (RType::GameInstance::DEBUG_LOGS)
                std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::PlayerPvPMode(ecs::Entity &bullet, float deltaTime)
{
    (void) deltaTime;
    auto bulletComp = bullet.getComponent<ecs::BulletComponent>();
    auto bulletPos = bullet.getComponent<ecs::PositionComponent>();
    auto bulletHitbox = bullet.getComponent<ecs::HitboxComponent>();
    const int DMG = 100;
    int team = bulletComp->getTeam();

    if (!bulletComp || !bulletPos || !bulletHitbox)
        return;

    for (size_t id : _game->refEntityManager.getCurrentLevel()
             .findEntitiesIdByComponent<ecs::PlayerComponent>()) {
        if (id == bullet.getID())
            continue;
        try {
            auto &enti =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);
            auto position2 = enti.getComponent<ecs::PositionComponent>();
            auto player2 = enti.getComponent<ecs::PlayerComponent>();
            auto health = enti.getComponent<ecs::HealthComponent>();
            auto playerHitb2 = enti.getComponent<ecs::HitboxComponent>();
            if (!position2 || !player2 || !health || !playerHitb2)
                continue;
            if (team == player2->getTeam())
                continue;
            if (isInBounds(bulletPos->getX(), bulletPos->getY(),
                    bulletHitbox->getWidth(), bulletHitbox->getHeight(),
                    position2->getX(), position2->getY(),
                    playerHitb2->getWidth(), playerHitb2->getHeight())) {
                if constexpr (server) {
                    _game->damagePlayer(player2->getPlayerID(), DMG);
                }
                _game->refEntityManager.getCurrentLevel()
                    .markEntityForDeletion(bullet.getID());
            }

        } catch (const std::exception &e) {
            if constexpr (RType::GameInstance::DEBUG_LOGS)
                std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}

void HitboxSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    (void) entities;
    for (size_t id : _game->refEntityManager.getCurrentLevel()
             .findEntitiesIdByComponent<ecs::PositionComponent>()) {
        try {
            auto &entity =
                _game->refEntityManager.getCurrentLevel().getEntityById(id);
            auto bullet = entity.getComponent<ecs::BulletComponent>();
            auto enemy = entity.getComponent<ecs::EnemyComponent>();
            auto bonus = entity.getComponent<ecs::BonusComponent>();
            auto player = entity.getComponent<ecs::PlayerComponent>();

            if (enemy && enemy->getWave() != _game->currentWave)
                continue;

            auto hitbox = entity.getComponent<ecs::HitboxComponent>();
            auto position = entity.getComponent<ecs::PositionComponent>();
            auto velocity = entity.getComponent<ecs::VelocityComponent>();

            if (!hitbox)
                continue;

            if (bullet && position && velocity && hitbox
                && bullet->getIsFromPlayer() == true
                && _game->getGameMode() == 0) {
                PlayerBulletCollision(entity);
            }
            if (bullet && position && velocity && hitbox
                && bullet->getIsFromPlayer() == false) {
                EnemyBulletCollision(entity);
            }
            if (_game->isServer() && enemy && position && velocity && hitbox) {
                EnemyCollision(entity, deltaTime);
            }
            if (bonus && position && velocity && hitbox && _game->isServer()) {
                BonusCollision(entity);
            }
            if (_game->getGameMode() == 1 && bullet && position && velocity
                && hitbox && bullet->getIsFromPlayer() == true)
                PlayerPvPMode(entity, deltaTime);
        } catch (const std::exception &e) {
            if constexpr (RType::GameInstance::DEBUG_LOGS)
                std::cout << CATCH_ERROR_LOCATION << e.what() << std::endl;
        }
    }
}
