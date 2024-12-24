/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** PositionSystem
*/

#include "Systems.hpp"

void ecs::PositionSystem::update(std::vector<ecs::Entity> &entities,
    sf::RenderWindow &window, float deltaTime, bool isServer)
{
    for (auto &entity : entities) {
        auto positionComponent = entity.getComponent<ecs::PositionComponent>();
        auto velocityComponent = entity.getComponent<ecs::VelocityComponent>();
        auto bullet = entity.getComponent<ecs::BulletComponent>();

        if (positionComponent && velocityComponent && !bullet) {
            float newX = static_cast<float>(positionComponent->getX())
                + static_cast<float>(velocityComponent->getVx()) * deltaTime;
            float newY = static_cast<float>(positionComponent->getY())
                + static_cast<float>(velocityComponent->getVy()) * deltaTime;

            positionComponent->setX(newX);
            positionComponent->setY(newY);

            if (!isServer) {
                auto sprite =
                    entity.getComponent<SpriteComponent<sf::Sprite>>();

                float maxX = 0;
                float maxY = 0;

                if (sprite) {
                    maxX = (float) window.getSize().x
                        - (float) sprite->getSprite().getTextureRect().width
                        - (float) sprite->getSprite().getScale().x;

                    maxY = (float) window.getSize().y
                        - (float) sprite->getSprite().getTextureRect().height
                        - (float) sprite->getSprite().getScale().y;
                }

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
}