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
        auto player = entity.getComponent<ecs::PlayerComponent>();
        auto bonus = entity.getComponent<ecs::BonusComponent>();
        auto enemy = entity.getComponent<ecs::EnemyComponent>();
        if ((enemy && enemy->getWave() != _game->currentWave )|| (!player && bonus && bonus->getWave() != _game->currentWave))
            continue;
        if (positionComponent && velocityComponent && !bullet) {
            float newX = static_cast<float>(positionComponent->getX())
                + static_cast<float>(velocityComponent->getVx()) * deltaTime;
            float newY = static_cast<float>(positionComponent->getY())
                + static_cast<float>(velocityComponent->getVy()) * deltaTime;
            positionComponent->setX(newX);
            positionComponent->setY(newY);

            auto ai = entity.getComponent<ecs::AIComponent>();
            if (player
                && (positionComponent->getOldX() != positionComponent->getX()
                    || positionComponent->getOldY()
                        != positionComponent->getY())) {
                _game->sendPlayerPosition(player->getPlayerID());
            }
            if constexpr (!server) {
                auto sprite =
                    entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();

                float maxX = 1;
                float maxY = 1;
                float minX = 0;
                float minY = 0;

                if (player || ai) {
                    maxX = 1.0f
                        - (((float) sprite->getSprite().getTextureRect().width
                               * (float) sprite->getSprite().getScale().x)
                            / (float) _game->getWindow().getSize().x / 2);
                    maxY = 1.0f
                        - (((float) sprite->getSprite().getTextureRect().height
                               * (float) sprite->getSprite().getScale().y)
                            / (float) _game->getWindow().getSize().y / 2);
                    minX = 0.0f
                        + (((float) sprite->getSprite().getTextureRect().width
                               * (float) sprite->getSprite().getScale().x)
                            / (float) _game->getWindow().getSize().x / 2);
                    minY = 0.0f
                        + (((float) sprite->getSprite().getTextureRect().height
                               * (float) sprite->getSprite().getScale().y)
                            / (float) _game->getWindow().getSize().y / 2);
                }

                if (player && positionComponent->getX() < minX) {
                    positionComponent->setX(minX);
                }
                if (player && positionComponent->getX() > maxX) {
                    positionComponent->setX(maxX);
                }
                if (player && positionComponent->getY() < minY) {
                    positionComponent->setY(minY);
                }
                if (player && positionComponent->getY() > maxY) {
                    positionComponent->setY(maxY);
                }
                if (ai) {
                    if (positionComponent->getX() > maxX) {
                        velocityComponent->setVx(
                            velocityComponent->getVx() * -1.0f);
                    }

                    if (positionComponent->getY() > maxY) {
                        velocityComponent->setVy(
                            velocityComponent->getVy() * -1.0f);
                    }

                    if (positionComponent->getX() < minX) {
                        velocityComponent->setVx(
                            velocityComponent->getVx() * -1.0f);
                    }

                    if (positionComponent->getY() < minY) {
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
