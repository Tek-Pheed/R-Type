/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#include "GameProtocol.hpp"
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

// If isLocalPlayer is set to false, then this function should only be
// triggered by a request from the server
ecs::Entity &RType::GameInstance::buildPlayer(bool isLocalPlayer, size_t id)
{
    std::cout << "Adding new player to the game" << std::endl;
    auto &player = refEntityManager.getCurrentLevel().createEntity();
    player.addComponent(std::make_shared<ecs::PlayerComponent>(id));
    player.addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player.addComponent(std::make_shared<ecs::HealthComponent>(100));
    player.addComponent(std::make_shared<ecs::VelocityComponent>(0, 0));
    if (!_isServer) {
        auto &texture =
            refAssetManager.getAsset<sf::Texture>(Asset::PLAYER_TEXTURE);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(66, 0, 33, 14));
        sprite.setScale(sf::Vector2f(3, 3));
        player.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        player.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 3.0, 3.0));
    }
    if (isLocalPlayer) {
        _playerEntityID = (int) player.getID();
    }
    if ((isLocalPlayer && _isConnectedToServer) || _isServer) {
        auto pos = player.getComponent<ecs::PositionComponent>();
        if (pos) {
            if (!isServer()) {
                refNetworkManager.sendToAll(
                    System::Network::ISocket::Type::TCP,
                    playerConnection(id, pos->getX(), pos->getY()));
            } else {
                refNetworkManager.sendToOthers(id,
                    System::Network::ISocket::Type::TCP,
                    playerConnection(id, pos->getX(), pos->getY()));
            }
        }
    }
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

void GameInstance::playerShoot(ecs::Entity &player)
{
    if (!hasLocalPlayer())
        return;

    std::stringstream ss;
    auto positionComp = player.getComponent<ecs::PositionComponent>();
    auto &bullet = refEntityManager.getCurrentLevel().createEntity();
    if (!positionComp)
        return;
    auto &texture =
        refAssetManager.getAsset<sf::Texture>(Asset::BULLET_TEXTURE);
    bullet.addComponent(std::make_shared<ecs::BulletComponent>(1));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(
        positionComp->getX() + 100, positionComp->getY() + 25));
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));
    bullet.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    sf::Sprite s;
    s.setTexture(texture);
    s.setTextureRect(sf::Rect(137, 153, 64, 16));
    bullet.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 132, 33));

    // ss << "104 " << _playerEntityID << "\t\n";

    // writeToServer(ss.str(), System::Network::ISocket::UDP);
}

void GameInstance::playerAnimations(ecs::Entity &player, std::string direction)
{
    auto renderComp = player.getComponent<ecs::SpriteComponent<sf::Sprite>>();
    if (direction == "top") {
        renderComp->getSprite().setTextureRect(sf::Rect(132, 0, 33, 14));
    } else if (direction == "down") {
        renderComp->getSprite().setTextureRect(sf::Rect(0, 0, 33, 14));
    } else {
        renderComp->getSprite().setTextureRect(sf::Rect(66, 0, 33, 14));
    }
}
