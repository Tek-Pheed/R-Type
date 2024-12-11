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

void client::createEnemy(std::vector<std::string> &tokens)
{
    (void) tokens;
    std::cout << "Create new enemy" << std::endl;
    // id, x, y
    const std::string name = "enemy";
    const int id = std::stoi(tokens[0]);
    const float x = std::stof(tokens[1]);
    const float y = std::stof(tokens[2]);

    if (_refRender.getEnemyTexture().getSize().x == 0) {
        sf::Texture enemyText;
        enemyText.loadFromFile("assets/sprites/r-typesheet31.gif");
        _refRender.setEnemyTexture(enemyText);
    }

    auto enemy = std::make_shared<ecs::Entity>(id);
    enemy->addComponent(std::make_shared<ecs::EnemyComponent>("Enemy"));
    enemy->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
    enemy->addComponent(
        std::make_shared<ecs::VelocityComponent>(-200.0f, 0.0f));
    enemy->addComponent(std::make_shared<ecs::HealthComponent>(100));

    enemy->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, _refRender.getEnemyTexture()));
    enemy->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
        sf::Rect(0, 16, 32, 32));
    enemy->getComponent<ecs::RenderComponent>()->getSprite()->setScale(
        sf::Vector2f(2, 2));

    add_entity(enemy);
}

void client::handle_enemy(int code, std::vector<std::string> &tokens)
{
    switch (code) {
        case E_SPAWN: createEnemy(tokens); break;
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
