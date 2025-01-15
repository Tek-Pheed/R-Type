/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildBullet.cpp
*/

#include <algorithm>
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
    constexpr auto velocity = 0.35f;
    const float Width = 0.042f * (float) _game.WinScaleX;
    const float Height = 0.022f * (float) _game.WinScaleY;

    auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
    auto &player = _game.getPlayerById(playerID);
    auto positionComp = player.getComponent<ecs::PositionComponent>();
    if (!positionComp) {
        _game.refEntityManager.getCurrentLevel().markEntityForDeletion(bullet.getID());
        return;
    }
    float x = positionComp->getX() + 0.05f;
    float y = positionComp->getY();

    if (_game.getGameMode() == 1 && player.getComponent<ecs::PlayerComponent>()
        && player.getComponent<ecs::PlayerComponent>()->getTeam() == 1)
        bullet.addComponent(
            std::make_shared<ecs::BulletComponent>(true, 0, 1));
    else
        bullet.addComponent(
            std::make_shared<ecs::BulletComponent>(true, 0, 0));
    if (_game.getGameMode() == 1 && player.getComponent<ecs::PlayerComponent>()
        && player.getComponent<ecs::PlayerComponent>()->getTeam() == 1)
        bullet.addComponent(
            std::make_shared<ecs::VelocityComponent>(-velocity, 0));
    else
        bullet.addComponent(
            std::make_shared<ecs::VelocityComponent>(velocity, 0));
    bullet.addComponent(
        std::make_shared<ecs::HitboxComponent>(0.042f, 0.022f));
    if (_game.getGameMode() == 1 && player.getComponent<ecs::PlayerComponent>()
        && player.getComponent<ecs::PlayerComponent>()->getTeam() == 1)
        x = positionComp->getX() - 0.05f;
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
        s.setScale(Width / s.getLocalBounds().width,
            Height / s.getLocalBounds().height);
        s.setOrigin(
            s.getLocalBounds().width / 2.0f, s.getLocalBounds().height / 2.0f);
        if (_game.getGameMode() == 1
            && player.getComponent<ecs::PlayerComponent>()
            && player.getComponent<ecs::PlayerComponent>()->getTeam() == 1) {
            s.setScale(-(Width / s.getLocalBounds().width), 1);
            s.setColor(sf::Color::Red);
        }
        bullet.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 0));
        auto &bulletSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
            Asset::BULLET_SOUND);
        static sf::Sound sound;
        sound.setBuffer(bulletSound);
        sound.setVolume(GameInstance::BULLET_VOLUME);
        sound.play();
        if (playerID == (size_t) _game.getNetClientID())
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
    }
}

void Factory::buildBulletFromEnemy(size_t enemyID)
{
    constexpr auto velocity = -0.30f;
    const float Width = 0.027f * (float) _game.WinScaleX;
    const float Height = 0.03f * (float) _game.WinScaleY;

    auto &enemy = _game.getEnemyById(enemyID);
    auto positionComp = enemy.getComponent<ecs::PositionComponent>();
    if (!positionComp)
        return;
    if (enemy.getComponent<ecs::EnemyComponent>()->getWave()
        != _game.currentWave)
        return;
    auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
    bullet.addComponent(std::make_shared<ecs::BulletComponent>(false));
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(velocity, 0));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(
        positionComp->getX(), positionComp->getY() + 0.034f));
    bullet.addComponent(std::make_shared<ecs::HitboxComponent>(0.027f, 0.03f));

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
        s.setTextureRect(sf::Rect(300, 410, 35, 22));
        s.setScale(Width / s.getLocalBounds().width,
            Height / s.getLocalBounds().height);
        s.setOrigin(
            s.getLocalBounds().width / 2.0f, s.getLocalBounds().height / 2.0f);
        bullet.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            s, 35, 0, 300 + (35 * 4), 0.1f, 300));
        auto &bulletSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
            Asset::BULLET_SOUND);
        static sf::Sound sound;
        sound.setBuffer(bulletSound);
        sound.setVolume(GameInstance::BULLET_VOLUME);
        sound.play();
        if (enemyID == (size_t) _game.getNetClientID())
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
    }
}

void Factory::buildBulletFromBoss(size_t bossId, float velx, float vely)
{
    const float bulletVelocity = -0.4f;
    const float Width = 0.1f * (float) _game.WinScaleY;
    const float Height = Width;

    auto &boss = _game.getEnemyById(bossId);
    auto positionComp = boss.getComponent<ecs::PositionComponent>();
    if (!positionComp)
        return;
    if (boss.getComponent<ecs::EnemyComponent>()->getWave()
        != _game.currentWave)
        return;
    auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
    bullet.addComponent(std::make_shared<ecs::BulletComponent>(false, 1));

    if (_game.isServer() && _game.getAllPlayers().size() != 0) {
        auto plv =
            _game.getRandomPlayer().getComponent<ecs::PositionComponent>();
        vely = std::clamp(plv->getY() - positionComp->getY(), bulletVelocity,
            -bulletVelocity);
        velx = std::clamp(plv->getX() - positionComp->getX(), bulletVelocity,
            -bulletVelocity);
    } else if (velx == 0.0f && vely == 0.0f) {
        velx = bulletVelocity;
    }
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(velx, vely));
    bullet.addComponent(std::make_shared<ecs::HitboxComponent>(0.1f, 0.1f));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(
        positionComp->getX() - 0.04f, positionComp->getY() - 0.065f));

    std::stringstream ss;
    ss << E_SHOOT << " " << _game.getTicks() << " " << bossId << " " << velx
       << " " << vely << " " << PACKET_END;
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
        s.setScale(Width / s.getLocalBounds().width,
            Height / s.getLocalBounds().height);
        s.setOrigin(
            s.getLocalBounds().width / 2.0f, s.getLocalBounds().height / 2.0f);
        bullet.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            s, 34, 0, 34 * 3, 0.05f, 0));
        auto &bulletSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
            Asset::BULLET_BOSS_SOUND);
        static sf::Sound sound;
        sound.setBuffer(bulletSound);
        sound.setVolume(GameInstance::BULLET_VOLUME);
        sound.play();
    }
}
