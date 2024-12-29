/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BulletSystem.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "GameSystems.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Game.hpp"

using namespace RType;
using namespace GameSystems;

void BulletSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
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
        if (position->getX() >= (float) _game->getWindow().getSize().x
            || position->getX() < 0
            || position->getY() >= (float) _game->getWindow().getSize().y
            || position->getY() < 0) {
            bulletToRemove.push_back(entity);
            continue;
        }
    }
    for (auto &bullet : bulletToRemove) {
        _game->entityManager.getCurrentLevel().destroyEntityById(bullet.getID());
    }
}
