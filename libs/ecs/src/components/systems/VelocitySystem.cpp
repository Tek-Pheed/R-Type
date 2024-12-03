/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** VelocitySystem
*/

#include "Systems.hpp"

ecs::VelocitySystem::VelocitySystem()
{
}

ecs::VelocitySystem::update(sf::Window *window, float deltaTime)
{
    (void)window;
    for (auto &entity : entities) {
        auto &velocity = entity.getComponent<VelocityComponent>();
        auto &position = entity.getComponent<PositionComponent>();

        position.setX(position.getX() + velocity.getVx() * deltaTime);
        position.setY(position.getY() + velocity.getVy() * deltaTime);
    }
}
