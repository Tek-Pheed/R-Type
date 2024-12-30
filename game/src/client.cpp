/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Graphics.hpp>
#include <memory>
#include <sstream>
#include "Components.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/VideoMode.hpp"

using namespace RType;

void RType::GameInstance::setupClient(
    const std::string &ip, uint16_t tcpPort, uint16_t udpPort)
{
    _isServer = false;
    _ip = ip;
    _tcpPort = tcpPort;
    _udpPort = udpPort;
    _window = std::make_unique<sf::RenderWindow>();
    sf::VideoMode videoMode(
        1280, 720, sf::VideoMode::getDesktopMode().bitsPerPixel);
    _window->create(videoMode, "R-Type");
    _window->setFramerateLimit(refGameEngine.getTickRate());
    if (!_window->isOpen()) {
        throw std::runtime_error("Failed to create the SFML window.");
    }
    loadTexture();
    createPersistentLevel();
    levelMainMenu();

    // Connect to server only when player select a game
    // networkManager.setupClient<RType::PacketHandler>(_tcpPort, _udpPort,
    // _ip);
}

sf::RenderWindow &GameInstance::getWindow()
{
    return *_window;
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
