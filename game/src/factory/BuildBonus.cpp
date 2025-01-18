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
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"

using namespace RType;

ecs::Entity &Factory::buildBonus(
    size_t id, float posX, float posY, int type, int wave)
{
    constexpr auto velocity = -0.08f;
    const float Width = 0.04f * (float) _game.WinScaleX;
    const float Height = Width;

    auto &bonusE = _game.refEntityManager.getCurrentLevel().createEntity();
    bonusE.addComponent(std::make_shared<ecs::BonusComponent>(
        id, static_cast<ecs::Bonus>(type), wave));
    bonusE.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    bonusE.addComponent(
        std::make_shared<ecs::VelocityComponent>(velocity, 0.0f));
    bonusE.addComponent(std::make_shared<ecs::HitboxComponent>(Width, Height));

    if constexpr (!server) {
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
                << pos->getY() << " " << wave << PACKET_END;

            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return bonusE;
}

ecs::Entity &GameInstance::getBonusById(size_t bonusID)
{
    auto bonuses = refEntityManager.getCurrentLevel()
                       .findEntitiesByComponent<ecs::BonusComponent>();

    for (auto &pl : bonuses) {
        if (pl.get().getComponent<ecs::BonusComponent>()->getBonusID()
            == bonusID) {
            return (pl);
        }
    }
    throw ErrorClass(
        THROW_ERROR_LOCATION "Bonus not found id=" + std::to_string(bonusID));
}

void GameInstance::applyBonus(
    ecs::Entity &bonus, size_t playerID, int bonusType)
{
    auto bonComp = bonus.getComponent<ecs::BonusComponent>();
    if (!bonComp)
        return;
    size_t bonusID = bonus.getComponent<ecs::BonusComponent>()->getBonusID();
    size_t bonusIDE = bonus.getID();
    auto &player = getPlayerById(playerID);
    auto playerComp = player.getComponent<ecs::PlayerComponent>();
    if (!playerComp)
        return;
    if constexpr (RType::GameInstance::DEBUG_LOGS)
        std::cout << "Bonus " << bonusID << " with type " << bonComp->getBonus()
                  << " applied to player " << playerComp->getPlayerID()
                  << std::endl;

    auto currentPlayerBonus = player.getComponent<ecs::BonusComponent>();
    auto health = player.getComponent<ecs::HealthComponent>();
    if (!currentPlayerBonus || !health)
        return;
    if (bonusType == 2)
        damagePlayer(
            player.getComponent<ecs::PlayerComponent>()->getPlayerID(), (-50));
    currentPlayerBonus->setBonus(static_cast<ecs::Bonus>(bonusType));
    refEntityManager.getCurrentLevel().markEntityForDeletion(bonusIDE);
    if constexpr (!server) {
        auto &bonusSound =
            refAssetManager.getAsset<sf::SoundBuffer>(Asset::BONUS_GET);
        factory.buildSoundEffect(bonusSound, "bonusSound", 100.0f);
    } else {
        std::stringstream sss;
        sss << BN_GET << " " << playerComp->getPlayerID() << " "
            << bonComp->getBonusID() << " " << bonusType << " " << PACKET_END;
        refNetworkManager.sendToAll(
            System::Network::ISocket::Type::TCP, sss.str());
    }
}

void GameInstance::handleNetworkBonuses(
    int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case Protocol::BN_SPAWN: {
            if (tokens.size() >= 3) {
                if constexpr (!server) {
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    std::shared_ptr<ecs::PositionComponent> pos;
                    factory.buildBonus(id,
                        (float) std::atof(tokens[1].c_str()),
                        (float) std::atof(tokens[2].c_str()), ecs::Bonus::NONE,
                        std::atoi(tokens[3].c_str()));
                }
            }
            break;
        }
        case Protocol::BN_GET: {
            if (tokens.size() >= 3) {
                size_t bonusID = (size_t) std::atoi(tokens[1].c_str());
                size_t playerID = (size_t) std::atoi(tokens[0].c_str());
                int bonusType = (int) std::atoi(tokens[2].c_str());
                auto &bon = getBonusById(bonusID);
                applyBonus(bon, playerID, bonusType);
            }
            break;
        }
        default: break;
    }
}
