/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** PositionSystem
*/

#include "Systems.hpp"

void ecs::PositionSystem::update(
    std::vector<std::shared_ptr<ecs::Entity>> &entities,
    sf::RenderWindow *window, float deltaTime)
{
    for (auto &entity : entities) {
        auto positionComponent =
            entity.get()->getComponent<ecs::PositionComponent>();
        auto velocityComponent =
            entity.get()->getComponent<ecs::VelocityComponent>();
        if (positionComponent && velocityComponent) {
            float newX = static_cast<float>(positionComponent->getX())
                + static_cast<float>(velocityComponent->getVx()) * deltaTime;
            float newY = static_cast<float>(positionComponent->getY())
                + static_cast<float>(velocityComponent->getVy()) * deltaTime;

            auto windowSize = window->getSize();
            float windowWidth = static_cast<float>(windowSize.x);
            float windowHeight = static_cast<float>(windowSize.y);

            if (newX >= 0 && newX <= windowWidth) {
                positionComponent->setX(newX);
            }
            if (newY >= 0 && newY <= windowHeight) {
                positionComponent->setY(newY);
            }
        }
    }
}
