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

using namespace RType;

ecs::Entity &RType::Factory::buildEnemy(
    size_t id, float posX, float posY, float health)
{
    std::cout << "Adding new enemy to the game at pos " << posX << " " << posY
              << std::endl;
    auto &enemy = _game.refEntityManager.getCurrentLevel().createEntity();
    enemy.addComponent(std::make_shared<ecs::EnemyComponent>(id));
    enemy.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    enemy.addComponent(std::make_shared<ecs::HealthComponent>(health));
    enemy.addComponent(
        std::make_shared<ecs::VelocityComponent>(-200.0f, 0.0f));
    enemy.addComponent(std::make_shared<ecs::HitboxComponent>(64.0f, 32.0f));

    if (!_game.isServer()) {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::ENEMY_TEXTURE);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 16, 32, 32));
        sprite.setScale(sf::Vector2f(2, 2));
        enemy.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        enemy.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 1.0, 1.0));
    } else {
        auto pos = enemy.getComponent<ecs::PositionComponent>();
        auto ene = enemy.getComponent<ecs::EnemyComponent>();
        if (pos) {
            std::stringstream sss;
            sss << E_SPAWN << " " << ene->getEnemyID() << " 0 " << pos->getX()
                << " " << pos->getY() << PACKET_END;
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
    throw ErrorClass("Enemy not found id=" + std::to_string(enemyID));
}

void GameInstance::sendEnemyPosition(size_t enemyID)
{
    if (isServer()) {
        std::unique_lock lock(_gameLock);
        auto &ene = getEnemyById(enemyID);
        auto position = ene.getComponent<ecs::PositionComponent>();
        std::stringstream ss;
        ss << E_POS << " " << _ticks << " " << enemyID << " "
           << position->getX() << " " << position->getY() << PACKET_END;

        refNetworkManager.sendToAll(
            System::Network::ISocket::Type::UDP, ss.str());
    }
}

void GameInstance::deleteEnemy(size_t enemyID)
{
    std::cout << "Deleting enemy" << std::endl;
    auto &ene = getEnemyById(enemyID);
    refEntityManager.getCurrentLevel().destroyEntityById(ene.getID());
    if (isServer()) {
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
            if (tokens.size() >= 4) {
                if (!isServer()) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    size_t type = (size_t) atoi(tokens[1].c_str());
                    std::shared_ptr<ecs::PositionComponent> pos;
                    std::cout << "Spawning enemy " << id << " with type "
                              << type << std::endl;
                    if (type == 0)
                        _factory.buildEnemy(id,
                            (float) std::atof(tokens[2].c_str()),
                            (float) std::atof(tokens[3].c_str()));
                    if (type == 1)
                        _factory.buildEnemyShooter(id,
                            (float) std::atof(tokens[2].c_str()),
                            (float) std::atof(tokens[3].c_str()));
                }
            }
            break;
        }
        case Protocol::E_POS: {
            if (tokens.size() >= 4) {
                if (!isServer()) {
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
            if (_isServer)
                return;
            if (tokens.size() >= 1) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                deleteEnemy(id);
            }
            break;
        }
        default: break;
    }
}

ecs::Entity &RType::Factory::buildEnemyShooter(
    size_t id, float posX, float posY, float health)
{
    std::cout << "Adding new enemy to the game" << std::endl;
    auto &enemy = _game.refEntityManager.getCurrentLevel().createEntity();
    enemy.addComponent(std::make_shared<ecs::EnemyComponent>(id));
    enemy.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    enemy.addComponent(std::make_shared<ecs::HealthComponent>(health));
    enemy.addComponent(
        std::make_shared<ecs::VelocityComponent>(-200.0f, 0.0f));
    enemy.addComponent(std::make_shared<ecs::HitboxComponent>(64.0f, 32.0f));

    if (!_game.isServer()) {
        auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
            Asset::SHOOTERENEMY_TEXTURE);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 16, 32, 32));
        sprite.setScale(sf::Vector2f(2, 2));
        enemy.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        enemy.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 1.0, 1.0));
    } else {
        auto pos = enemy.getComponent<ecs::PositionComponent>();
        auto ene = enemy.getComponent<ecs::EnemyComponent>();
        if (pos) {
            std::stringstream sss;
            sss << E_SPAWN << " " << ene->getEnemyID() << " 1 " << pos->getX()
                << " " << pos->getY() << PACKET_END;
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return (enemy);
}
