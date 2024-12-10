/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#include <FLAC/format.h>
#include "Entity.hpp"
#include "client.hpp"
#include "protocol.hpp"

void client::create_new_player(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);
    const float x = std::stof(tokens[1]);
    const float y = std::stof(tokens[2]);
    sf::Texture playerTexture;

    playerTexture.loadFromFile("../../assets/sprites/r-typesheet42.gif");

    auto player = std::make_shared<ecs::Entity>(id);
    player->addComponent(std::make_shared<ecs::PlayerComponent>(tokens[1]));
    player->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));

    player->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, playerTexture));
    _entities.push_back(player);
}

void client::set_new_position(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);
    const float x = std::stof(tokens[1]);
    const float y = std::stof(tokens[2]);

    for (auto &entity : _entities) {
        if (entity->getID() == static_cast<size_t>(id)) {
            auto playerComp = entity->getComponent<ecs::PlayerComponent>();
            if (playerComp == nullptr)
                return;
            auto position = entity->getComponent<ecs::PositionComponent>();
            position->setX(static_cast<float>(x));
            position->setY(static_cast<float>(y));
        }
    }
}

void client::player_dead(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);

    for (auto &entity : _entities) {
        if (entity->getID() == static_cast<size_t>(id)) {
            auto playerComp = entity->getComponent<ecs::PlayerComponent>();
            if (playerComp == nullptr)
                return;
            _entities.erase(
                std::remove(_entities.begin(), _entities.end(), entity),
                _entities.end());
        }
    }
}

void client::create_projectile(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);
    float x = 0.0f;
    float y = 0.0f;

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("./assets/sprites/r-typesheet1.gif");

    for (auto &entity : _entities) {
        if (entity->getID() == static_cast<size_t>(id)) {
            auto playerComp = entity->getComponent<ecs::PlayerComponent>();
            if (playerComp == nullptr)
                return;
            auto position = entity->getComponent<ecs::PositionComponent>();
            x = position->getX();
            y = position->getY();
            break;
        }
    }

    auto bullet = std::make_shared<ecs::Entity>(rand());
    bullet->addComponent(std::make_shared<ecs::BulletComponent>(1));
    bullet->addComponent(
        std::make_shared<ecs::PositionComponent>(x + 100, y + 25));
    bullet->addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));

    bullet->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, bulletTexture));

    bullet->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
        sf::Rect(137, 153, 64, 16));
    _entities.push_back(bullet);
}

void client::set_player_health(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);
    const int dmg = std::stoi(tokens[1]);

    for (auto &entity : _entities) {
        if (entity->getID() == static_cast<size_t>(id)) {
            auto playerComp = entity->getComponent<ecs::PlayerComponent>();
            if (playerComp == nullptr)
                return;
            auto health = entity->getComponent<ecs::HealthComponent>();
            health->setHealth(health->getHealth() - dmg);
        }
    }
}

void client::player_disconnection(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);

    for (auto &entity : _entities) {
        if (entity->getID() == static_cast<size_t>(id)) {
            auto playerComp = entity->getComponent<ecs::PlayerComponent>();
            if (playerComp == nullptr)
                return;
            _entities.erase(
                std::remove(_entities.begin(), _entities.end(), entity),
                _entities.end());
        }
    }
}

void client::handle_player(int code, std::vector<std::string> &tokens)
{
    (void) tokens;
    switch (code) {
        case P_CONN: {
            create_new_player(tokens);
            break;
        }
        case P_POS: {
            set_new_position(tokens);
            break;
        }
        case P_DEAD: {
            player_dead(tokens);
            break;
        }
        case P_SHOOT: {
            // TODO spawn bullet
            break;
        }
        case P_DMG: {
            set_player_health(tokens);
            break;
        }
        case P_DISCONN: {
            player_disconnection(tokens);
            break;
        }
        default: {
            break;
        }
    }
}
