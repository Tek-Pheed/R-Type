/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** enemy
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <sstream>
#include "Components.hpp"
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "SFML/Audio/SoundSource.hpp"
#include "SFML/Graphics/Texture.hpp"

using namespace RType;

ecs::Entity &RType::Factory::buildEnemy(size_t id, float posX, float posY,
    float health, int wave, float velocityX, float velocityY)
{
    const float Width = 0.05f * (float) _game.WinScaleX;
    const float Height = Width;

    if constexpr (RType::GameInstance::DEBUG_LOGS)
        std::cout << "Adding new enemy (" << id << ") to the game at pos "
                  << posX << " " << posY << std::endl;
    auto &enemy = _game.refEntityManager.getCurrentLevel().createEntity();
    enemy.addComponent(std::make_shared<ecs::EnemyComponent>(id));
    enemy.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    enemy.addComponent(
        std::make_shared<ecs::HealthComponent>(static_cast<float>(health)
            * static_cast<float>(_game.getDifficulty())));
    enemy.addComponent(std::make_shared<ecs::VelocityComponent>(
        GameInstance::ENEMY_VELOCITY, 0.0f));
    enemy.addComponent(std::make_shared<ecs::HitboxComponent>(0.05f, 0.05f));
    enemy.getComponent<ecs::EnemyComponent>()->setWave(wave);
    enemy.getComponent<ecs::VelocityComponent>()->setVx(velocityX);
    enemy.getComponent<ecs::VelocityComponent>()->setVy(velocityY);
    if constexpr (!server) {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::ENEMY_TEXTURE);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 16, 32, 32));
        sprite.setScale(Width / sprite.getLocalBounds().width,
            Height / sprite.getLocalBounds().height);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
            sprite.getLocalBounds().height / 2.0f);
        enemy.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        enemy.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 0));
    } else {
        auto pos = enemy.getComponent<ecs::PositionComponent>();
        auto ene = enemy.getComponent<ecs::EnemyComponent>();
        if (pos) {
            std::stringstream sss;
            sss << E_SPAWN << " " << ene->getEnemyID() << " 0 " << pos->getX()
                << " " << pos->getY() << " " << health << " " << wave << " "
                << velocityX << " " << velocityY << PACKET_END;
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return (enemy);
}

ecs::Entity &GameInstance::getEnemyById(size_t enemyID)
{
    std::unique_lock lock(_gameLock);
    auto enemies = refEntityManager.getCurrentLevel()
                       .findEntitiesByComponent<ecs::EnemyComponent>();

    for (auto &pl : enemies) {
        if (pl.get().getComponent<ecs::EnemyComponent>()->getEnemyID()
            == enemyID)
            return (pl.get());
    }
    throw ErrorClass(
        THROW_ERROR_LOCATION "Enemy not found id=" + std::to_string(enemyID));
}

void GameInstance::sendEnemyPosition(size_t enemyID)
{
    if constexpr (server) {
        std::unique_lock lock(_gameLock);
        auto &ene = getEnemyById(enemyID);
        auto position = ene.getComponent<ecs::PositionComponent>();

        // Check if in frame (saves bandwith)
        if (position->getX() > KILLZONE
            && position->getX() < 1.0f - (float) KILLZONE
            && position->getY() > KILLZONE
            && position->getY() < 1.0f - (float) KILLZONE) {
            std::stringstream ss;
            ss << E_POS << " " << _ticks << " " << enemyID << " "
               << position->getX() << " " << position->getY() << PACKET_END;

            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
        }
    }
}

void GameInstance::deleteEnemy(size_t enemyID)
{
    std::unique_lock lock(_gameLock);
    if constexpr (RType::GameInstance::DEBUG_LOGS)
        std::cout << "Deleting enemy: " << enemyID << std::endl;
    auto &ene = getEnemyById(enemyID);
    size_t eneid = ene.getID();
    if constexpr (!server)
        factory.buildExplosionEnemy(
            ene.getComponent<ecs::PositionComponent>()->getX(),
            ene.getComponent<ecs::PositionComponent>()->getY());
    refEntityManager.getCurrentLevel().markEntityForDeletion(eneid);
    if constexpr (server) {
        std::stringstream ss;
        ss << E_DEAD << " " << enemyID << " " << PACKET_END;
        refNetworkManager.sendToAll(
            System::Network::ISocket::Type::TCP, ss.str());
    }
}

void GameInstance::handleNetworkEnemies(
    int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case Protocol::E_SPAWN: {
            if (tokens.size() >= 6) {
                if constexpr (!server) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    size_t type = (size_t) atoi(tokens[1].c_str());
                    std::shared_ptr<ecs::PositionComponent> pos;
                    if constexpr (RType::GameInstance::DEBUG_LOGS)
                        std::cout << "Spawning enemy " << id << " with type "
                                  << type << std::endl;
                    if (type == 0 && tokens.size() >= 8)
                        factory.buildEnemy(id,
                            (float) std::atof(tokens[2].c_str()),
                            (float) std::atof(tokens[3].c_str()),
                            (float) std::atof(tokens[4].c_str()),
                            std::atoi(tokens[5].c_str()),
                            (float) std::atof(tokens[6].c_str()),
                            (float) std::atof(tokens[7].c_str()));
                    if (type == 1 && tokens.size() >= 8)
                        factory.buildEnemyShooter(id,
                            (float) std::atof(tokens[2].c_str()),
                            (float) std::atof(tokens[3].c_str()),
                            (float) std::atof(tokens[4].c_str()),
                            std::atoi(tokens[5].c_str()),
                            (float) std::atof(tokens[6].c_str()),
                            (float) std::atof(tokens[7].c_str()));
                    if (type == 2)
                        factory.buildBoss(id,
                            (float) std::atof(tokens[2].c_str()),
                            (float) std::atof(tokens[3].c_str()),
                            (float) std::atof(tokens[4].c_str()),
                            std::atoi(tokens[5].c_str()));
                }
            }
            break;
        }
        case Protocol::E_POS: {
            if (tokens.size() >= 4) {
                if constexpr (!server) {
                    uint64_t tick = (uint64_t) atoll(tokens[0].c_str());
                    if (_lastNetTick <= tick) {
                        _lastNetTick = tick;
                        size_t id = (size_t) atoi(tokens[1].c_str());
                        auto &ene = getEnemyById(id);
                        auto pos = ene.getComponent<ecs::PositionComponent>();
                        pos->setX((float) std::atof(tokens[2].c_str()));
                        pos->setY((float) std::atof(tokens[3].c_str()));
                    }
                }
            }
            break;
        }
        case Protocol::E_DEAD: {
            std::unique_lock lock(_gameLock);
            if constexpr (server)
                return;
            if (tokens.size() >= 1) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                deleteEnemy(id);
            }
            break;
        }
        case Protocol::E_SHOOT: {
            if constexpr (server)
                return;
            if (tokens.size() >= 2) {
                uint64_t tick = (uint64_t) atoll(tokens[0].c_str());
                if (_lastNetTick <= tick) {
                    if constexpr (RType::GameInstance::DEBUG_LOGS)
                        std::cout << "Enemy shoot" << std::endl;
                    _lastNetTick = tick;
                    size_t id = (size_t) atoi(tokens[1].c_str());
                    std::unique_lock lock(_gameLock);
                    auto mob = getEnemyById(id);
                    size_t type =
                        mob.getComponent<ecs::EnemyComponent>()->getType();
                    if (type == 1)
                        factory.buildBulletFromEnemy(id);
                    else if (type == 2) {
                        float velx = (float) std::atof(tokens[2].c_str());
                        float vely = (float) std::atof(tokens[3].c_str());
                        factory.buildBulletFromBoss(id, velx, vely);
                    }
                }
            }
        }
        default: break;
    }
}

ecs::Entity &RType::Factory::buildEnemyShooter(size_t id, float posX,
    float posY, float health, int wave, float velocityX, float velocityY)
{
    const float Width = 0.05f * (float) _game.WinScaleX;
    const float Height = 0.120f * (float) _game.WinScaleY;

    if constexpr (RType::GameInstance::DEBUG_LOGS)
        std::cout << "Adding new enemy (" << id << ") to the game at pos "
                  << posX << " " << posY << std::endl;
    auto &enemy = _game.refEntityManager.getCurrentLevel().createEntity();
    enemy.addComponent(std::make_shared<ecs::EnemyComponent>(id, 1));
    enemy.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    enemy.addComponent(std::make_shared<ecs::HealthComponent>(health));
    enemy.addComponent(std::make_shared<ecs::VelocityComponent>(
        GameInstance::ENEMY_SHOOTER_VELOCITY, 0.0f));
    enemy.addComponent(std::make_shared<ecs::HitboxComponent>(0.05f, 0.120f));
    enemy.getComponent<ecs::EnemyComponent>()->setWave(wave);
    enemy.getComponent<ecs::VelocityComponent>()->setVx(velocityX);
    enemy.getComponent<ecs::VelocityComponent>()->setVy(velocityY);
    if constexpr (!server) {
        auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
            Asset::SHOOTERENEMY_TEXTURE);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 0, 33, 34));
        sprite.setScale(Width / sprite.getLocalBounds().width,
            Height / sprite.getLocalBounds().height);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
            sprite.getLocalBounds().height / 2.0f);
        enemy.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        enemy.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 33, 0, 33 * 3, 0.1f, 0));
        buildBulletFromEnemy(id);
    } else {
        auto pos = enemy.getComponent<ecs::PositionComponent>();
        auto ene = enemy.getComponent<ecs::EnemyComponent>();
        if (pos) {
            std::stringstream sss;
            sss << E_SPAWN << " " << ene->getEnemyID() << " 1 " << pos->getX()
                << " " << pos->getY() << " " << health << " " << wave << " "
                << velocityX << " " << velocityY << PACKET_END;
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return (enemy);
}
