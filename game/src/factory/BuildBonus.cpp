/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildBonus.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Components.hpp"
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"

using namespace RType;

ecs::Entity &Factory::buildBonus(
    size_t id, float posX, float posY, ecs::Bonus bonus)
{
    auto &bonusE = _game.refEntityManager.getCurrentLevel().createEntity();

    bonusE.addComponent(std::make_shared<ecs::BonusComponent>(id, bonus));
    bonusE.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    bonusE.addComponent(
        std::make_shared<ecs::VelocityComponent>(-100.0f, 0.0f));
    bonusE.addComponent(
        std::make_shared<ecs::HitboxComponent>(17.0f * 3, 18.0f * 3));

    if (!_game.isServer()) {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::BONUS_TEXTURE);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 0, 17, 18));
        sprite.setScale(sf::Vector2f(3, 3));
        bonusE.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        bonusE.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 17, 0, 205, 0.1f, 0));
    } else {
        auto pos = bonusE.getComponent<ecs::PositionComponent>();
        auto ene = bonusE.getComponent<ecs::BonusComponent>();
        if (pos) {
            std::stringstream sss;
            sss << BN_SPAWN << " " << id << " " << pos->getX() << " "
                << pos->getY() << PACKET_END;
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return bonusE;
}

void GameInstance::handleNetworkBonuses(
    int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case Protocol::BN_SPAWN: {
            if (tokens.size() >= 3) {
                if (!isServer()) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    std::shared_ptr<ecs::PositionComponent> pos;
                    _factory.buildBonus(id,
                        (float) std::atof(tokens[1].c_str()),
                        (float) std::atof(tokens[2].c_str()),
                        ecs::Bonus::NONE);
                }
            }
            break;
        }
        default: break;
    }
}