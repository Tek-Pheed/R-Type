/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildBullet.cpp
*/

#include <sstream>
#include "Components.hpp"
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"

namespace RType
{
    void Factory::buildBulletFromPlayer(size_t playerID)
    {
        auto player = _game.getPlayerById(playerID);
        auto positionComp = player.getComponent<ecs::PositionComponent>();
        if (!positionComp)
            return;
        auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
        float x = positionComp->getX() + 100;
        float y = positionComp->getY() + 25;

        bullet.addComponent(std::make_shared<ecs::BulletComponent>(1));
        bullet.addComponent(
            std::make_shared<ecs::VelocityComponent>(0.35f, 0));
        bullet.addComponent(std::make_shared<ecs::HitboxComponent>(54, 16));
        bullet.addComponent(std::make_shared<ecs::PositionComponent>(
            positionComp->getX() + 0.05, positionComp->getY() + 0.025));

        std::stringstream ss;
        ss << P_SHOOT << " " << playerID << " " << PACKET_END;
        if (_game.isServer()) {
            _game.refNetworkManager.sendToOthers(
                playerID, System::Network::ISocket::Type::UDP, ss.str());
        } else {
            auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
                Asset::BULLET_TEXTURE);
            bullet.addComponent(std::make_shared<ecs::RenderComponent>(
                ecs::RenderComponent::ObjectType::SPRITE));
            sf::Sprite s;

            s.setTexture(texture);
            s.setTextureRect(sf::Rect(137, 153, 64, 16));
            bullet.addComponent(
                std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 1, 1));
            auto &bulletSound =
                _game.refAssetManager.getAsset<sf::SoundBuffer>(
                    Asset::BULLET_SOUND);
            static sf::Sound sound;
            sound.setBuffer(bulletSound);
            sound.setVolume(25.0f);
            sound.play();
            if (playerID == (size_t) _game.getNetClientID())
                _game.refNetworkManager.sendToAll(
                    System::Network::ISocket::Type::UDP, ss.str());
        }
    }
} // namespace RType
