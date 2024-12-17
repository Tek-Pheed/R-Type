/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include <ctime>
#include <memory>
#include <vector>
#include "Entity.hpp"
#include "Networking.hpp"
#include "RenderClass.hpp"
#include "game.hpp"
#include "system_network.hpp"

game::game(Networking &net)
    : _id(-1), _refNetwork(net)
{
}

game::~game()
{
    _id = -1;
    _refRender->~RenderClass();
}

void game::setRenderClass(RenderClass *refRender)
{
    _refRender = refRender;
}

void game::loadTexture()
{
    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture bulletTexture;

    if (playerTexture.loadFromFile("assets/sprites/r-typesheet42.gif"))
        _refRender->setPlayerTexture(playerTexture);
    if (enemyTexture.loadFromFile("assets/sprites/r-typesheet31.gif"))
        _refRender->setEnemyTexture(enemyTexture);
    if (bulletTexture.loadFromFile("assets/sprites/r-typesheet1.gif"))
        _refRender->setBulletTexture(bulletTexture);
}

void game::addEntity(std::shared_ptr<ecs::Entity> entity)
{
    _entities.push_back(entity);
}

std::vector<std::shared_ptr<ecs::Entity>> &game::getEntities()
{
    return _entities;
}

int game::getId()
{
    return _id;
}

void game::createPlayer()
{
    auto &player = getLocalPlayer();

    player = std::make_shared<ecs::Entity>(getId());
    player->addComponent(std::make_shared<ecs::PlayerComponent>("Samy"));
    player->addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    player->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, _refRender->getPlayerTexture()));
    player->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
        sf::Rect(66, 0, 33, 14));
    player->getComponent<ecs::RenderComponent>()->getSprite()->setScale(
        sf::Vector2f(3, 3));
    addEntity(player);
}

RenderClass *game::getRenderClass()
{
    return _refRender;
}
