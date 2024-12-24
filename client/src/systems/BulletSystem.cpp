/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BulletSystem.cpp
*/

#include "Systems.hpp"

void ecs::BulletSystem::update(std::vector<ecs::Entity> &entities,
    sf::RenderWindow &window, float deltaTime, bool isServer)
{
    std::vector<ecs::Entity> bulletToRemove;

    for (auto &entity : entities) {
        auto bullet = entity.getComponent<ecs::BulletComponent>();

        if (bullet && !isServer) {
            auto position = entity.getComponent<ecs::PositionComponent>();
            auto velocity = entity.getComponent<ecs::VelocityComponent>();

            position->setX(position->getX() + velocity->getVx() * deltaTime);
            position->setY(position->getY() + velocity->getVy() * deltaTime);

            if (position->getX() >= (float) window.getSize().x
                || position->getX() < 0
                || position->getY() >= (float) window.getSize().y
                || position->getY() < 0) {
                bulletToRemove.push_back(entity);
                continue;
            }
        }
    }

    /*for (auto &bullet : bulletToRemove) {
        entities.erase(std::remove(entities.begin(), entities.end(), bullet),
            entities.end());
    }*/
}