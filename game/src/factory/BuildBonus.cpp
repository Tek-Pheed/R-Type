/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildBonus.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <sstream>
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
    constexpr auto velocity = -0.08f;
    const float Width = 0.04f * (float) _game.WindoScaleX;
    const float Height = Width;

    auto &bonusE = _game.refEntityManager.getCurrentLevel().createEntity();
    bonusE.addComponent(std::make_shared<ecs::BonusComponent>(id, bonus));
    bonusE.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    bonusE.addComponent(
        std::make_shared<ecs::VelocityComponent>(velocity, 0.0f));
    bonusE.addComponent(std::make_shared<ecs::HitboxComponent>(Width, Height));

    if (!_game.isServer()) {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::BONUS_TEXTURE);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 0, 17, 18));
        sprite.setScale(Width / sprite.getLocalBounds().width,
            Height / sprite.getLocalBounds().height);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
            sprite.getLocalBounds().height / 2.0f);
        bonusE.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        bonusE.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 17, 0, 17 * 12, 0.1f, 0));
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
        case Protocol::BN_GET: {
            if (tokens.size() >= 3) {
                std::cout << "Bonus " << tokens[1].c_str() << " "
                          << tokens[2].c_str() << " get by "
                          << tokens[0].c_str() << std::endl;
                auto bonuses =
                    refEntityManager.getCurrentLevel()
                        .findEntitiesByComponent<ecs::BonusComponent>();
                size_t bonusID = (size_t) std::atof(tokens[1].c_str());
                size_t bonusIDE = 0;
                for (auto &pl : bonuses) {
                    if (pl.get()
                            .getComponent<ecs::BonusComponent>()
                            ->getBonusID()
                        == bonusID) {
                        bonusIDE = pl.get().getID();
                    }
                }
                refEntityManager.getCurrentLevel().markEntityForDeletion(
                    bonusIDE);
                auto &currentPlayer = getLocalPlayer();
                if (currentPlayer.getComponent<ecs::PlayerComponent>()
                        ->getPlayerID()
                    == (size_t) std::atoi(tokens[0].c_str())) {
                    auto currentPlayerBonus =
                        currentPlayer.getComponent<ecs::BonusComponent>();
                    if (std::atoi(tokens[2].c_str()) == 3) {
                        auto health =
                            currentPlayer.getComponent<ecs::HealthComponent>();
                        damagePlayer(
                            currentPlayer.getComponent<ecs::PlayerComponent>()
                                ->getPlayerID(),
                            (health->getHealth() - 100));
                    }
                    currentPlayerBonus->setBonus(
                        static_cast<ecs::Bonus>(std::atoi(tokens[2].c_str())));
                    std::cout << "set bonus " << currentPlayerBonus->getBonus()
                              << std::endl;
                }
            }
            break;
        }
        default: break;
    }
}