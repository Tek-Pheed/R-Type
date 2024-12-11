/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BulletSystem.cpp
*/

#include "Systems.hpp"

void ecs::BulletSystem::update(
    std::vector<std::shared_ptr<ecs::Entity>> &entities,
    sf::RenderWindow *window, float deltaTime, bool isServer)
{
    std::vector<std::shared_ptr<ecs::Entity>> bulletToRemove;

    for (auto &entity : entities) {
        auto bullet = entity->getComponent<ecs::BulletComponent>();

        if (bullet) {
            auto position = entity->getComponent<ecs::PositionComponent>();
            auto velocity = entity->getComponent<ecs::VelocityComponent>();

            if (position && velocity) {
                position->setX(
                    position->getX() + velocity->getVx() * deltaTime);
                position->setY(
                    position->getY() + velocity->getVy() * deltaTime);

                if (position->getX() >= float(window->getSize().x && !isServer)
                    || position->getX() < 0) {
                    bulletToRemove.push_back(entity);
                    continue;
                }
            }
        }
    }

    for (auto &bullet : bulletToRemove) {
        entities.erase(std::remove(entities.begin(), entities.end(), bullet),
            entities.end());
    }
}