/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildBullet.cpp
*/

#include <mutex>
#include <sstream>
#include "Components.hpp"
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"

using namespace RType;

void Factory::buildBulletFromPlayer(size_t playerID)
{
    auto &player = _game.getPlayerById(playerID);
    auto positionComp = player.getComponent<ecs::PositionComponent>();
    if (!positionComp)
        return;
    auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
    float x = positionComp->getX() + 100;
    float y = positionComp->getY() + 25;

    bullet.addComponent(std::make_shared<ecs::BulletComponent>(true));
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));
    bullet.addComponent(std::make_shared<ecs::HitboxComponent>(54, 16));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(x, y));

    std::stringstream ss;
    ss << P_SHOOT << " " << playerID << PACKET_END;
    if (_game.isServer()) {
        _game.refNetworkManager.sendToOthers(
            playerID, System::Network::ISocket::Type::UDP, ss.str());
    } else {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::BULLET_TEXTURE);
        bullet.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        sf::Sprite s;

        s.setTexture(texture);
        s.setTextureRect(sf::Rect(137, 153, 64, 16));
        bullet.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 1, 1));
        auto &bulletSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
            Asset::BULLET_SOUND);
        static sf::Sound sound;
        sound.setBuffer(bulletSound);
        sound.setVolume(10.0f);
        sound.play();
        if (playerID == (size_t) _game.getNetClientID())
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
    }
}

void Factory::buildBulletFromEnemy(size_t enemyID)
{
    auto &enemy = _game.getEnemyById(enemyID);
    auto positionComp = enemy.getComponent<ecs::PositionComponent>();
    if (!positionComp)
        return;
    auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
    bullet.addComponent(std::make_shared<ecs::BulletComponent>(false));
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(-350.0f, 0));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(
        positionComp->getX(), positionComp->getY() + 25));
    bullet.addComponent(std::make_shared<ecs::HitboxComponent>(33, 21));

    std::stringstream ss;
    ss << E_SHOOT << " " << _game.getTicks() << " " << enemyID << PACKET_END;
    if (_game.isServer()) {
        _game.refNetworkManager.sendToAll(
            System::Network::ISocket::Type::UDP, ss.str());
    } else {
        auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
            Asset::BULLETENEMY_TEXTURE);
        bullet.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        sf::Sprite s;
        s.setTexture(texture);
        s.setTextureRect(sf::Rect(335, 412, 33, 21));
        bullet.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 132, 33));
        auto &bulletSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
            Asset::BULLET_SOUND);
        static sf::Sound sound;
        sound.setBuffer(bulletSound);
        sound.setVolume(10.0f);
        sound.play();
        if (enemyID == (size_t) _game.getNetClientID())
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
    }
}

void Factory::buildBulletFromBoss(size_t bossId)
{
    auto &boss = _game.getEnemyById(bossId);
    auto positionComp = boss.getComponent<ecs::PositionComponent>();
    if (!positionComp)
        return;
    auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
    bullet.addComponent(std::make_shared<ecs::BulletComponent>(false, 1));
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(-350.0f, 0));
    bullet.addComponent(std::make_shared<ecs::HitboxComponent>(34, 34));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(
        positionComp->getX() + 20, positionComp->getY() + 100));

    std::stringstream ss;
    ss << E_SHOOT << " " << _game.getTicks() << " " << bossId << PACKET_END;
    if (_game.isServer()) {
        _game.refNetworkManager.sendToAll(
            System::Network::ISocket::Type::UDP, ss.str());
    } else {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::BULLET_BOSS);
        bullet.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        sf::Sprite s;
        s.setTexture(texture);
        s.setTextureRect(sf::Rect(0, 0, 34, 34));
        s.setScale(sf::Vector2f(2, 2));
        bullet.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 2, 2));
        auto &bulletSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
            Asset::BULLET_BOSS_SOUND);
        static sf::Sound sound;
        sound.setBuffer(bulletSound);
        sound.setVolume(25.0f);
        sound.play();
        if (bossId == (size_t) _game.getNetClientID())
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
    }
}
