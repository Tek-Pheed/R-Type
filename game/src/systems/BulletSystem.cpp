/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BulletSystem.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Components.hpp"
#include "Game.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

using namespace RType;
using namespace GameSystems;

constexpr auto bulletMaxTravel = 1.1f;

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
        if constexpr (server) {
            if (position->getX() >= (float) bulletMaxTravel
                || position->getX() < 0.0f
                || position->getY() >= (float) bulletMaxTravel
                || position->getY() < 0.0f) {
                bulletToRemove.push_back(entity);
                continue;
            }
        } else {
            if (position->getX() >= 1.0f || position->getX() < 0.0f
                || position->getY() >= 1.0f || position->getY() < 0.0f) {
                bulletToRemove.push_back(entity);
                continue;
            }
        }
    }
    for (auto &bl : bulletToRemove) {
        _game->refEntityManager.getCurrentLevel().markEntityForDeletion(
            bl.getID());
    }
}
