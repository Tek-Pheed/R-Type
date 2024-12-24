/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** PositionSystem
*/

#include "Components.hpp"
#include "GameSystems.hpp"

void PositionSystem::initSystem(sf::RenderWindow *win)
{
    _win = win;
}

void PositionSystem::update(
    std::vector<ecs::Entity> &entities, float deltaTime, bool isServer)
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
                    entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();

                float maxX = 0;
                float maxY = 0;

                if (sprite) {
                    maxX = (float) _win->getSize().x
                        - (float) sprite->getSprite().getTextureRect().width
                        - (float) sprite->getSprite().getScale().x;

                    maxY = (float) _win->getSize().y
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
