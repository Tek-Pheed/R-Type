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
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "SFML/Graphics/Texture.hpp"

using namespace RType;

ecs::Entity &RType::Factory::buildBoss(
    size_t id, float posX, float posY, float health, int wave)
{
    const float Width = 0.3f * (float) _game.WinScaleX;
    const float Height = 0.7f * (float) _game.WinScaleY;

    if constexpr (RType::GameInstance::DEBUG_LOGS)
        std::cout << "Adding new boss to the game" << std::endl;
    auto &boss = _game.refEntityManager.getCurrentLevel().createEntity();
    boss.addComponent(std::make_shared<ecs::EnemyComponent>(id, 2));
    boss.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    boss.addComponent(
        std::make_shared<ecs::HealthComponent>(static_cast<float>(health)
            * static_cast<float>(_game.getDifficulty())));
    boss.addComponent(std::make_shared<ecs::VelocityComponent>(0.0f, 0.0f));
    boss.addComponent(std::make_shared<ecs::HitboxComponent>(0.3f, 0.7f));
    boss.getComponent<ecs::EnemyComponent>()->setWave(wave);
    if constexpr (!server) {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::BOSS_TEXTURE);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(20, 640, 162, 207));
        sprite.setScale(Width / sprite.getLocalBounds().width,
            Height / sprite.getLocalBounds().height);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
            sprite.getLocalBounds().height / 2.0f);
        boss.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        boss.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite));
        boss.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 162, 0, 20 + (162 * 4), 0.25f, 20));
    } else {
        auto pos = boss.getComponent<ecs::PositionComponent>();
        auto ene = boss.getComponent<ecs::EnemyComponent>();
        if (pos) {
            std::stringstream sss;
            sss << E_SPAWN << " " << ene->getEnemyID() << " 2 " << pos->getX()
                << " " << pos->getY() << " " << health << " " << wave
                << PACKET_END;
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
    throw ErrorClass(
        THROW_ERROR_LOCATION "Boss not found id=" + std::to_string(bossID));
}

void GameInstance::sendBossPosition(size_t bossID)
{
    if constexpr (server) {
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
    if constexpr (RType::GameInstance::DEBUG_LOGS)
        std::cout << "Deleting boss" << std::endl;
    auto &ene = getBossById(bossID);
    refEntityManager.getCurrentLevel().markEntityForDeletion(ene.getID());
    if constexpr (server) {
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
            if (tokens.size() >= 6) {
                if constexpr (!server) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    std::shared_ptr<ecs::PositionComponent> pos;
                    factory.buildBoss(id,
                        (float) std::atof(tokens[1].c_str()),
                        (float) std::atof(tokens[2].c_str()),
                        (float) std::atof(tokens[3].c_str()),
                        (int) std::atoi(tokens[4].c_str()));
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
            if constexpr (server)
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
