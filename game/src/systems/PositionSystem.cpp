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
#include "components/AIComponent.hpp"

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
        auto enemy = entity.getComponent<ecs::EnemyComponent>();
        if (enemy && enemy->getWave() != _game->currentWave)
            continue;
        if (positionComponent && velocityComponent && !bullet) {
            float newX = static_cast<float>(positionComponent->getX())
                + static_cast<float>(velocityComponent->getVx()) * deltaTime;
            float newY = static_cast<float>(positionComponent->getY())
                + static_cast<float>(velocityComponent->getVy()) * deltaTime;
            positionComponent->setX(newX);
            positionComponent->setY(newY);

            auto player = entity.getComponent<ecs::PlayerComponent>();
            auto ai = entity.getComponent<ecs::AIComponent>();
            if (player
                && (positionComponent->getOldX() != positionComponent->getX()
                    || positionComponent->getOldY()
                        != positionComponent->getY())) {
                _game->sendPlayerPosition(player->getPlayerID());
            }
            if (!this->_game->isServer()) {
                auto sprite =
                    entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();

                float minX = (float) 0;
                float minY = (float) 0;

                if (sprite) {
                    minX = (float) sprite->getSprite().getTextureRect().width
                        * (float) sprite->getSprite().getScale().x / 2;

                    minY = (float) sprite->getSprite().getTextureRect().height
                        * (float) sprite->getSprite().getScale().y / 2;
                }

                if (player && positionComponent->getX() < minX) {
                    positionComponent->setX(minX);
                }
                if (player && positionComponent->getX() > 1.0f) {
                    positionComponent->setX(1.0f);
                }
                if (player && positionComponent->getY() < minY) {
                    positionComponent->setY(minY);
                }
                if (player && positionComponent->getY() > 1.0f) {
                    positionComponent->setY(1.0f);
                }
                if (ai) {
                    if (positionComponent->getX() > 1.0f) {
                        velocityComponent->setVx(
                            velocityComponent->getVx() * -1.0f);
                    }

                    if (positionComponent->getY() > 1.0f) {
                        velocityComponent->setVy(
                            velocityComponent->getVy() * -1.0f);
                    }

                    if (positionComponent->getX() < 0) {
                        velocityComponent->setVx(
                            velocityComponent->getVx() * -1.0f);
                    }

                    if (positionComponent->getY() < 0) {
                        velocityComponent->setVy(
                            velocityComponent->getVy() * -1.0f);
                    }
                }

            } else {
                if (enemy && positionComponent
                    && positionComponent->getX() < GameInstance::KILLZONE) {
                    positionComponent->setX(1.0f - GameInstance::KILLZONE);
                }
                if (enemy && positionComponent
                    && positionComponent->getY() < GameInstance::KILLZONE) {
                    positionComponent->setY(1.0f - GameInstance::KILLZONE);
                }
                if (enemy && positionComponent
                    && positionComponent->getY()
                        > GameInstance::DEFAULT_RESOLUTION_Y
                    && positionComponent->getY()
                        > GameInstance::DEFAULT_RESOLUTION_Y
                            - GameInstance::KILLZONE) {
                    positionComponent->setY(GameInstance::KILLZONE);
                }
                if (enemy
                    && (positionComponent->getOldX()
                            != positionComponent->getX()
                        || positionComponent->getOldY()
                            != positionComponent->getY())) {
                    _game->sendEnemyPosition(enemy->getEnemyID());
                }
            }
        }
    }
}
