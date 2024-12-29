// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** client
// */

// #include <ctime>
// #include <memory>
// #include <vector>
// #include "Entity.hpp"
// #include "Networking.hpp"
// #include "RenderClass.hpp"
// #include "Game.hpp"
// #include "system_network.hpp"

// game::game(Networking &net)
//     : _id(-1), _refNetwork(net)
// {
// }

// game::~game()
// {
//     _id = -1;
//     _refRender->~RenderClass();
// }

// void game::setRenderClass(RenderClass *refRender)
// {
//     _refRender = refRender;
// }

// void game::loadTexture()
// {
//     sf::Texture playerTexture;
//     sf::Texture enemyTexture;
//     sf::Texture bulletTexture;

//     if (playerTexture.loadFromFile("assets/sprites/r-typesheet42.gif"))
//         _refRender->setPlayerTexture(playerTexture);
//     if (enemyTexture.loadFromFile("assets/sprites/r-typesheet31.gif"))
//         _refRender->setEnemyTexture(enemyTexture);
//     if (bulletTexture.loadFromFile("assets/sprites/r-typesheet1.gif"))
//         _refRender->setBulletTexture(bulletTexture);
// }

// void game::addEntity(ecs::Entity & entity)
// {
//     _entities.push_back(entity);
// }

// std::vector<ecs::Entity &> &game::getEntities()
// {
//     return _entities;
// }

// int game::getId()
// {
//     return _id;
// }

// RenderClass *game::getRenderClass()
// {
//     return _refRender;
// }

#include <SFML/Graphics.hpp>
#include <sstream>

#include "Components.hpp"
#include "EngineECSManager.hpp"
#include "Entity.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Game.hpp"

using namespace RType;

void GameInstance::createPlayer()
{
    auto &player = entityManager.getCurrentLevel().createEntity();
    auto &texture = assetManager.getAsset<sf::Texture>("playerTexture");
    sf::Sprite sprite;

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::Rect(66, 0, 33, 14));
    sprite.setScale(sf::Vector2f(3, 3));
    player.addComponent(std::make_shared<ecs::PlayerComponent>());
    player.addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    player.addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    player.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 3.0, 3.0));
}

sf::RenderWindow &GameInstance::getWindow()
{
    return *_window;
}

void GameInstance::playerShoot(ecs::Entity &player)
{
    std::stringstream ss;
    auto &manager =
        refGameEngine.getFeature<Engine::Feature::ECSManager<GameInstance>>();
    auto bullet = manager.createEntity();
    auto positionComp = player.getComponent<ecs::PositionComponent>();

    bullet.addComponent(std::make_shared<ecs::BulletComponent>(1));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(
        positionComp->getX() + 100, positionComp->getY() + 25));
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));
    bullet.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    sf::Sprite s;
    bullet.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 132, 33));

    bullet.getComponent<ecs::SpriteComponent<sf::Sprite>>()
        ->getSprite()
        .setTextureRect(sf::Rect(137, 153, 64, 16));
    ss << "104 " << _PlayerId << "\t\n";

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
