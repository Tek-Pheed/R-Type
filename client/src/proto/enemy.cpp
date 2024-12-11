/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** enemy
*/

#include <memory>
#include "Components.hpp"
#include "Entity.hpp"
#include "RenderClass.hpp"
#include "client.hpp"
#include "protocol.hpp"

void client::create_enemy(std::vector<std::string> &tokens)
{
    std::cout << "Create new enemy" << std::endl;
    // id, x, y
    const std::string name = "enemy";
    const int id = std::stoi(tokens[0]);
    const float x = std::stof(tokens[1]);
    const float y = std::stof(tokens[2]);

    auto enemy = std::make_shared<ecs::Entity>(id);
    enemy->addComponent(std::make_shared<ecs::EnemyComponent>());
    enemy->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
    enemy->addComponent(
        std::make_shared<ecs::VelocityComponent>(-200.0f, 0.0f));
    enemy->addComponent(std::make_shared<ecs::HealthComponent>(100));

    /*enemy->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, _refRender.getEnemyTexture()));
    enemy->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
        sf::Rect(66, 0, 33, 14));
    enemy->getComponent<ecs::RenderComponent>()->getSprite()->setScale(
        sf::Vector2f(3, 3));*/

    /*auto player = std::make_shared<ecs::Entity>(id);
    player->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    player->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, _refRender.getPlayerTexture()));
    player->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
        sf::Rect(66, 0, 33, 14));
    player->getComponent<ecs::RenderComponent>()->getSprite()->setScale(
        sf::Vector2f(3, 3));*/

    add_entity(enemy);
}

void client::handle_enemy(int code, std::vector<std::string> &tokens)
{
    (void) tokens;
    switch (code) {
        case E_SPAWN: create_enemy(tokens); break;
        case E_POS:
            // code
            break;
        case E_DEAD:
            // code
            break;
        case E_SHOOT:
            // code
            break;
        case E_DMG:
            // code
            break;
        default: break;
    }
}
