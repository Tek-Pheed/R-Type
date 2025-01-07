/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildBullet.cpp
*/

#include <sstream>
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"

namespace RType
{
    void Factory::buildBulletFromPlayer(size_t playerID)
    {
        auto player = _game.getPlayerById(playerID);
        auto positionComp = player.getComponent<ecs::PositionComponent>();
        if (!positionComp)
            return;
        auto &bullet = _game.refEntityManager.getCurrentLevel().createEntity();
        bullet.addComponent(std::make_shared<ecs::BulletComponent>(1));
        bullet.addComponent(
            std::make_shared<ecs::VelocityComponent>(350.0f, 0));
        bullet.addComponent(std::make_shared<ecs::PositionComponent>(
            positionComp->getX() + 100, positionComp->getY() + 25));

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
                std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
                    s, 132, 33));
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