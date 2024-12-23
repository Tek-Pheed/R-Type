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

    sf::Sprite enemySP(_refRender.getEnemyTexture());

    auto enemy = std::make_shared<ecs::Entity>(id);
    enemy->addComponent(std::make_shared<ecs::EnemyComponent>());
    enemy->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
    enemy->addComponent(
        std::make_shared<ecs::VelocityComponent>(-200.0f, 0.0f));
    enemy->addComponent(std::make_shared<ecs::HealthComponent>(100));

    enemySP.setTextureRect(sf::Rect(0, 16, 32, 32));
    enemySP.setScale(sf::Vector2f(2, 2));

    enemy->addComponent(
        std::make_shared<ecs::RenderComponent>(ecs::ObjectType::SPRITE));
    enemy->addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(enemySP, 2, 2));

    add_entity(enemy);
}

void client::enemyDead(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);

    for (auto &entity : _entities) {
        if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
            auto enemyComp = entity->getComponent<ecs::EnemyComponent>();
            if (enemyComp == nullptr)
                return;
            _entities.erase(
                std::remove(_entities.begin(), _entities.end(), entity),
                _entities.end());
        }
    }
}

void client::enemyShoot(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);
    float x = 0.0f;
    float y = 0.0f;

    for (auto &entity : _entities) {
        if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
            auto enemyComp = entity->getComponent<ecs::EnemyComponent>();
            if (enemyComp == nullptr)
                return;
            auto position = entity->getComponent<ecs::PositionComponent>();
            x = position->getX();
            y = position->getY();
            break;
        }
    }

    auto bullet = std::make_shared<ecs::Entity>(rand());
    bullet->addComponent(std::make_shared<ecs::BulletComponent>(0));
    bullet->addComponent(
        std::make_shared<ecs::PositionComponent>(x + 100, y + 25));
    bullet->addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));

    bullet->addComponent(
        std::make_shared<ecs::RenderComponent>(ecs::ObjectType::SPRITE));

    sf::Sprite bulletSP(_refRender.getBulletTexture());
    bulletSP.setTextureRect(sf::Rect(137, 153, 64, 16));

    bullet->addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(bulletSP, 1, 1));
    _entities.push_back(bullet);
}

void client::enemyDamage(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);
    const int dmg = std::stoi(tokens[1]);

    for (auto &entity : _entities) {
        if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
            auto enemyComp = entity->getComponent<ecs::EnemyComponent>();
            if (enemyComp == nullptr)
                return;
            auto health = entity->getComponent<ecs::HealthComponent>();
            health->setHealth(health->getHealth() - dmg);
        }
    }
}

void client::handle_enemy(int code, std::vector<std::string> &tokens)
{
    switch (code) {
        case E_SPAWN: createEnemy(tokens); break;
        case E_POS:
            // code
            break;
        case E_DEAD: enemyDead(tokens); break;
        case E_SHOOT: enemyShoot(tokens); break;
        case E_DMG: enemyDamage(tokens); break;
        default: break;
    }
}
