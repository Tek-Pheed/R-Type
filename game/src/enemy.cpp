/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** enemy
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <memory>
#include <sstream>
#include <string>
#include "Components.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "system_network.hpp"

using namespace RType;

// Maybe move networking entity position update into the systems

ecs::Entity &RType::GameInstance::buildEnemy(
    size_t id, float posX, float posY, float health)
{
    std::cout << "Adding new enemy to the game" << std::endl;
    auto &enemy = refEntityManager.getCurrentLevel().createEntity();
    enemy.addComponent(std::make_shared<ecs::EnemyComponent>(id));
    enemy.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    enemy.addComponent(std::make_shared<ecs::HealthComponent>(health));
    enemy.addComponent(
        std::make_shared<ecs::VelocityComponent>(-200.0f, 0.0f));
    if (!_isServer) {
        auto &texture =
            refAssetManager.getAsset<sf::Texture>(Asset::ENEMY_TEXTURE);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 16, 32, 32));
        sprite.setScale(sf::Vector2f(2, 2));
        enemy.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        enemy.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 3.0, 3.0));
    }
    if (_isServer) {
        auto pos = enemy.getComponent<ecs::PositionComponent>();
        auto ene = enemy.getComponent<ecs::EnemyComponent>();
        if (pos) {
            std::stringstream sss;
            sss << E_SPAWN << " " << ene->getEnemyID() << " " << pos->getX()
                << " " << pos->getY() << PACKET_END;
            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return (enemy);
}

ecs::Entity &GameInstance::getEnemyById(size_t enemyID)
{
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
        auto &ene = getEnemyById(enemyID);
        auto position = ene.getComponent<ecs::PositionComponent>();
        std::stringstream ss;
        ss << E_POS << " " << enemyID << " " << position->getX() << " "
           << position->getY() << PACKET_END;

        refNetworkManager.sendToAll(
            System::Network::ISocket::Type::UDP, ss.str());
    }
}

void GameInstance::deleteEnemy(size_t enemyID)
{
    if (isServer() || _isConnectedToServer) {
        auto &ene = getEnemyById(enemyID);
        refEntityManager.getCurrentLevel().destroyEntityById(ene.getID());
        if (isServer()) {
            std::stringstream ss;
            ss << E_DEAD << " " << enemyID << " " << PACKET_END;
            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
        }
    }
}

void GameInstance::handleNetworkEnemies(
    int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case Protocol::E_SPAWN: {
            if (tokens.size() >= 3) {
                if (!isServer()) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    std::shared_ptr<ecs::PositionComponent> pos;
                    buildEnemy(id, (float) std::atof(tokens[1].c_str()),
                        (float) std::atof(tokens[2].c_str()));
                }
            }
            break;
        }
        case Protocol::E_POS: {
            if (tokens.size() >= 3) {
                if (!isServer()) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    auto &ene = getEnemyById(id);
                    auto pos = ene.getComponent<ecs::PositionComponent>();
                    pos->setX((float) std::atof(tokens[1].c_str()));
                    pos->setY((float) std::atof(tokens[2].c_str()));
                }
            }
            break;
        }
        case Protocol::E_DEAD: {
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
