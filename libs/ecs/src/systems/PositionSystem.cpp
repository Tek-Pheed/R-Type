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
            entity->getComponent<ecs::PositionComponent>();
        auto velocityComponent =
            entity->getComponent<ecs::VelocityComponent>();
        auto bullet = entity->getComponent<ecs::BulletComponent>();
        auto render = entity->getComponent<ecs::RenderComponent>();
        if (positionComponent && velocityComponent && !bullet && render) {
            float newX = static_cast<float>(positionComponent->getX())
                + static_cast<float>(velocityComponent->getVx()) * deltaTime;
            float newY = static_cast<float>(positionComponent->getY())
                + static_cast<float>(velocityComponent->getVy()) * deltaTime;

            positionComponent->setX(newX);
            positionComponent->setY(newY);

            float maxX = float(window->getSize().x)
                - float(render->getSprite()->getTextureRect().width)
                - float(render->getSprite()->getScale().x);

            float maxY = float(window->getSize().y)
                - float(render->getSprite()->getTextureRect().height)
                - float(render->getSprite()->getScale().y);

            if (positionComponent->getX() < 0) {
                positionComponent->setX(0);
            }
            if (positionComponent->getX() > maxX) {
                positionComponent->setX(maxX);
            }
            if (positionComponent->getY() < 0) {
                positionComponent->setY(0);
            }
            if (positionComponent->getY() > maxY) {
                positionComponent->setY(maxY);
            }
        }
    }
}
