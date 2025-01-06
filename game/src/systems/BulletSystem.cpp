/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BulletSystem.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Game.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

using namespace RType;
using namespace GameSystems;

constexpr auto bulletMaxTravel = 1500U;

void BulletSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

void BulletSystem::collision(ecs::Entity &bullet)
{
    auto bulletPos = bullet.getComponent<ecs::PositionComponent>();
    auto bulletComp = bullet.getComponent<ecs::BulletComponent>();
    auto bulletHitB = bullet.getComponent<ecs::HitboxComponent>();
    const float hitbox = 5.0f;

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
        if (bulletPos->getX()
                < position->getX() + enemyHitB->getWidth() + hitbox
            && bulletPos->getX() + bulletHitB->getWidth()
                > position->getX() - hitbox
            && bulletPos->getY()
                < position->getY() + enemyHitB->getHeight() + hitbox
            && bulletPos->getY() + bulletHitB->getHeight()
                > position->getY() - hitbox) {
            if (_game->isServer())
                health->setHealth(health->getHealth() - 101);
            _game->refEntityManager.getCurrentLevel().destroyEntityById(
                bullet.getID());
        }
    }
}

void BulletSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    std::vector<ecs::Entity> bulletToRemove;

    for (auto &entity : entities) {
        auto bullet = entity.getComponent<ecs::BulletComponent>();
        auto position = entity.getComponent<ecs::PositionComponent>();
        auto velocity = entity.getComponent<ecs::VelocityComponent>();
        if (!bullet || !position || !velocity)
            continue;
        position->setX(position->getX() + velocity->getVx() * deltaTime);
        position->setY(position->getY() + velocity->getVy() * deltaTime);
        if (_game->isServer()) {
            if (position->getX() >= (float) bulletMaxTravel
                || position->getX() < 0
                || position->getY() >= (float) bulletMaxTravel
                || position->getY() < 0) {
                bulletToRemove.push_back(entity);
                continue;
            }
        } else {
            if (position->getX() >= (float) _game->getWindow().getSize().x
                || position->getX() < 0
                || position->getY() >= (float) _game->getWindow().getSize().y
                || position->getY() < 0) {
                bulletToRemove.push_back(entity);
                continue;
            }
        }
        collision(entity);
    }
    for (auto &bl : bulletToRemove) {
        _game->refEntityManager.getCurrentLevel().destroyEntityById(
            bl.getID());
    }
}
