/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** boss
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
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "Game.hpp"

using namespace RType;

ecs::Entity &RType::Factory::buildBoss(
    size_t id, float posX, float posY, float health)
{
    std::cout << "Adding new boss to the game" << std::endl;
    auto &boss = _game.refEntityManager.getCurrentLevel().createEntity();
    boss.addComponent(std::make_shared<ecs::BossComponent>(id));
    boss.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    boss.addComponent(std::make_shared<ecs::HealthComponent>(health));
    boss.addComponent(std::make_shared<ecs::BulletComponent>(false));
    boss.addComponent(
        std::make_shared<ecs::VelocityComponent>(0.0f, 0.0f));
    if (!_game.isServer()) {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::BOSS_TEXTURE);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 0, 183, 207));
        sprite.setScale(sf::Vector2f(2, 2));
        boss.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        boss.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 3.0, 3.0));
    }
    if (_game.isServer()) {
        auto pos = boss.getComponent<ecs::PositionComponent>();
        auto ene = boss.getComponent<ecs::BossComponent>();
        if (pos) {
            std::stringstream sss;
            sss << B_SPAWN << " " << ene->getBossID() << " " << pos->getX()
                << " " << pos->getY() << PACKET_END;
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return (boss);
}

ecs::Entity &GameInstance::getBossById(size_t bossID)
{
    std::unique_lock lock(_gameLock);
    auto enemies = refEntityManager.getCurrentLevel()
                       .findEntitiesByComponent<ecs::BossComponent>();

    for (auto &pl : enemies) {
        if (pl.get().getComponent<ecs::BossComponent>()->getBossID() == bossID)
            return (pl.get());
    }
    throw ErrorClass("Boss not found id=" + std::to_string(bossID));
}

void GameInstance::sendBossPosition(size_t bossID)
{
    if (isServer()) {
        std::unique_lock lock(_gameLock);
        auto &ene = getBossById(bossID);
        auto position = ene.getComponent<ecs::PositionComponent>();
        std::stringstream ss;
        ss << E_POS << " " << _ticks << " " << bossID << " "
           << position->getX() << " " << position->getY() << PACKET_END;

        refNetworkManager.sendToAll(
            System::Network::ISocket::Type::UDP, ss.str());
    }
}

void GameInstance::deleteBoss(size_t bossID)
{
    std::unique_lock lock(_gameLock);
    std::cout << "Deleting boss" << std::endl;
    auto &ene = getBossById(bossID);
    refEntityManager.getCurrentLevel().destroyEntityById(ene.getID());
    if (isServer()) {
        std::stringstream ss;
        ss << E_DEAD << " " << bossID << " " << PACKET_END;
        refNetworkManager.sendToAll(
            System::Network::ISocket::Type::TCP, ss.str());
    }
}

void GameInstance::handleNetworkBosses(
    int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case Protocol::E_SPAWN: {
            if (tokens.size() >= 3) {
                if (!isServer()) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    std::shared_ptr<ecs::PositionComponent> pos;
                    _factory.buildBoss(id,
                        (float) std::atof(tokens[1].c_str()),
                        (float) std::atof(tokens[2].c_str()));
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
                        auto &ene = getBossById(id);
                        auto pos = ene.getComponent<ecs::PositionComponent>();
                        pos->setX((float) std::atof(tokens[2].c_str()));
                        pos->setY((float) std::atof(tokens[3].c_str()));
                    }
                }
            }
            break;
        }
        case Protocol::E_DEAD: {
            if (_isServer)
                return;
            if (tokens.size() >= 1) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                deleteBoss(id);
            }
            break;
        }
        default: break;
    }
}