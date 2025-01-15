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
#include "components/AIComponent.hpp"

namespace RType
{
    ecs::Entity &Factory::buildPlayer(
        bool isLocalPlayer, size_t id, const std::string &name)
    {
        const float Width = 0.075f * (float) _game.WinScaleX;
        const float Height = 0.06f * (float) _game.WinScaleY;

        if (RType::GameInstance::DEBUG_LOGS)
            std::cout << "Adding new player to the game" << std::endl;
        auto &player = _game.refEntityManager.getCurrentLevel().createEntity();
        player.addComponent(std::make_shared<ecs::PlayerComponent>(id));
        player.addComponent(
            std::make_shared<ecs::PositionComponent>(0.1f, 0.1f));
        if (_game.getGameMode() == 1)
            player.addComponent(std::make_shared<ecs::HealthComponent>(300));
        else
            player.addComponent(std::make_shared<ecs::HealthComponent>(100));
        player.addComponent(std::make_shared<ecs::VelocityComponent>(0, 0));
        player.addComponent(
            std::make_shared<ecs::HitboxComponent>(0.075f, 0.06f));
        std::string n = DEFAULT_PLAYER_NAME;
        if (!name.empty())
            n = name;
        if (!_game.isServer()) {
            auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
                Asset::PLAYER_TEXTURE);
            auto &font =
                _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT);
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::Rect(66, (17 * (int) (id % 5)), 33, 18));
            sprite.setScale(Width / sprite.getLocalBounds().width,
                Height / sprite.getLocalBounds().height);
            sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
                sprite.getLocalBounds().height / 2.0f);
            player.addComponent(std::make_shared<ecs::RenderComponent>(
                ecs::RenderComponent::ObjectType::SPRITEANDTEXT));
            player.addComponent(
                std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 0));

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(35);
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(1.0f);
            text.setString(n);
            player.addComponent(
                std::make_shared<ecs::TextComponent<sf::Text>>(text, n));
            player.addComponent(
                std::make_shared<ecs::BonusComponent>(ecs::Bonus::NONE));
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
                    << pos->getY() << " " << n << " " << PACKET_END;
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

    ecs::Entity &Factory::buildAIPlayer(
        sf::Vector2f velocity, const std::string &name, std::size_t skinID)
    {
        const float Width = 0.075f * (float) _game.WinScaleX;
        const float Height = 0.06f * (float) _game.WinScaleY;

        auto &player = _game.refEntityManager.getCurrentLevel().createEntity();
        player.addComponent(
            std::make_shared<ecs::PositionComponent>(0.1f, 0.1f));
        player.addComponent(
            std::make_shared<ecs::VelocityComponent>(velocity.x, velocity.y));
        if (!_game.isServer()) {
            auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
                Asset::PLAYER_TEXTURE);
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::Rect(66, (int) (17 * skinID), 33, 18));
            sprite.setScale(Width / sprite.getLocalBounds().width,
                Height / sprite.getLocalBounds().height);
            sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
                sprite.getLocalBounds().height / 2.0f);
            player.addComponent(std::make_shared<ecs::RenderComponent>(
                ecs::RenderComponent::ObjectType::SPRITEANDTEXT));
            player.addComponent(
                std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 0));

            auto &font =
                _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT);
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(35);
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(1.0f);
            text.setString(name);
            player.addComponent(
                std::make_shared<ecs::TextComponent<sf::Text>>(text, name));
            player.addComponent(std::make_shared<ecs::AIComponent>());
        }
        return (player);
    }
} // namespace RType
