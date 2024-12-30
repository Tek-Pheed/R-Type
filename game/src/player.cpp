/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <sstream>
#include "Components.hpp"
#include "ErrorClass.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameSystems.hpp"

using namespace RType;

ecs::Entity &RType::GameInstance::buildPlayer(bool isLocalPlayer, size_t id)
{
    auto &player = refEntityManager.getCurrentLevel().createEntity();
    auto &texture =
        refAssetManager.getAsset<sf::Texture>(Asset::PLAYER_TEXTURE);
    sf::Sprite sprite;

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::Rect(66, 0, 33, 14));
    sprite.setScale(sf::Vector2f(3, 3));
    player.addComponent(std::make_shared<ecs::PlayerComponent>(id));
    player.addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    player.addComponent(std::make_shared<ecs::VelocityComponent>(0, 0));
    player.addComponent(std::make_shared<ecs::HealthComponent>(100));
    player.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 3.0, 3.0));
    if (isLocalPlayer)
        _playerEntityID = (int) player.getID();
    return (player);
}

bool GameInstance::hasLocalPlayer(void) const
{
    if (_playerEntityID == -1) {
        return (false);
    }
    return (true);
}

ecs::Entity &GameInstance::getLocalPlayer()
{
    if (!hasLocalPlayer())
        throw ErrorClass("No player was attached to the client");
    return (refEntityManager.getCurrentLevel().getEntityById(
        (size_t) _playerEntityID));
}

std::vector<std::reference_wrapper<ecs::Entity>> GameInstance::getAllPlayers()
{
    return (refEntityManager.getCurrentLevel()
            .findEntitiesByComponent<ecs::PlayerComponent>());
}

void GameInstance::updateLocalPlayerPosition()
{
    if (!hasLocalPlayer())
        return;
    auto position = getLocalPlayer().getComponent<ecs::PositionComponent>();
    if (position) {
        float oldX = position->getOldX();
        float oldY = position->getOldY();
        float x = position->getX();
        float y = position->getY();

        if (oldX != x || oldY != y) {
            std::stringstream ss;
            ss << "102 " << _playerEntityID << " " << x << " " << y << "\t\n";
            // writeToServer(ss.str(), System::Network::ISocket::UDP);
        }
    }
}