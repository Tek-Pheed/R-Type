/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildPlayer.cpp
*/

#include <memory>
#include <sstream>
#include "Components.hpp"
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"

namespace RType
{
    ecs::Entity &Factory::buildPlayer(
        bool isLocalPlayer, size_t id, const std::string &name)
    {
        std::cout << "Adding new player to the game" << std::endl;
        auto &player = _game.refEntityManager.getCurrentLevel().createEntity();
        player.addComponent(std::make_shared<ecs::PlayerComponent>(id));
        player.addComponent(
            std::make_shared<ecs::PositionComponent>(100, 100));
        player.addComponent(std::make_shared<ecs::HealthComponent>(100));
        player.addComponent(std::make_shared<ecs::VelocityComponent>(0, 0));
        player.addComponent(
            std::make_shared<ecs::HitboxComponent>(33 * 3, 14 * 3));
        std::string n = DEFAULT_PLAYER_NAME;
        if (name != "")
            n = name;
        if (!_game.isServer()) {
            auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
                Asset::PLAYER_TEXTURE);
            auto &font =
                _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT);
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::Rect(66, 0, 33, 14));
            sprite.setScale(sf::Vector2f(3, 3));
            player.addComponent(std::make_shared<ecs::RenderComponent>(
                ecs::RenderComponent::ObjectType::SPRITEANDTEXT));
            player.addComponent(
                std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 0));

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(20);
            text.setString(n);
            player.addComponent(
                std::make_shared<ecs::TextComponent<sf::Text>>(text, n));
        } else {
            player.addComponent(
                std::make_shared<ecs::TextComponent<std::string>>(n, name));
        }
        if (isLocalPlayer) {
            _game.setPlayerEntityID((int) player.getID());
        }
        if ((isLocalPlayer && _game.isConnectedToServer())
            || _game.isServer()) {
            auto pos = player.getComponent<ecs::PositionComponent>();
            if (pos) {
                std::stringstream sss;
                sss << P_CONN << " " << id << " " << pos->getX() << " "
                    << pos->getY() << " " << n << PACKET_END;
                if (!_game.isServer()) {
                    _game.refNetworkManager.sendToAll(
                        System::Network::ISocket::Type::TCP, sss.str());
                } else {
                    _game.refNetworkManager.sendToOthers(
                        id, System::Network::ISocket::Type::TCP, sss.str());
                }
            }
        }
        return (player);
    }
} // namespace RType
