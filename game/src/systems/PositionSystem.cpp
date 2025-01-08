/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** PositionSystem
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Components.hpp"
#include "Game.hpp"
#include "GameSystems.hpp"

using namespace RType;
using namespace GameSystems;

void PositionSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

void PositionSystem::update(
    std::vector<ecs::Entity> &entities, float deltaTime)
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

            auto player = entity.getComponent<ecs::PlayerComponent>();
            if (player
                && (positionComponent->getOldX() != positionComponent->getX()
                    || positionComponent->getOldY()
                        != positionComponent->getY())) {
                _game->sendPlayerPosition(player->getPlayerID());
            }
            if (!this->_game->isServer()) {
                auto sprite =
                    entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();

                float maxX = (float) this->_game->getWindow().getSize().x;
                float maxY = (float) this->_game->getWindow().getSize().y;

                if (sprite) {
                    maxX = (float) _game->getWindow().getSize().x
                        - (float) sprite->getSprite().getTextureRect().width
                            * (float) sprite->getSprite().getScale().x;

                    maxY = (float) _game->getWindow().getSize().y
                        - (float) sprite->getSprite().getTextureRect().height
                            * (float) sprite->getSprite().getScale().y;
                }

                if (positionComponent->getX() < 0) {
                    positionComponent->setX(0);
                }
                if (positionComponent->getX()
                        * (float) _game->getWindow().getSize().x
                    > maxX) {
                    positionComponent->setX(
                        maxX / (float) _game->getWindow().getSize().x);
                }
                if (positionComponent->getY() < 0) {
                    positionComponent->setY(0);
                }
                if (positionComponent->getY()
                        * (float) _game->getWindow().getSize().y
                    > maxY) {
                    positionComponent->setY(
                        maxY / (float) _game->getWindow().getSize().y);
                }
            } else {
                auto enemy = entity.getComponent<ecs::EnemyComponent>();
                if (enemy
                    && (positionComponent->getOldX()
                            != positionComponent->getX()
                        || positionComponent->getOldY()
                            != positionComponent->getY())) {
                    _game->sendEnemyPosition(enemy->getEnemyID());
                }
                if (enemy
                    && (positionComponent->getX() < 0.0f
                        || positionComponent->getY() < 0.0f)) {
                    _game->deleteEnemy(enemy->getEnemyID());
                }
            }
        }
    }
}
