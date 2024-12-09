/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include <iostream>
#include <memory>
#include <vector>
#include "Entity.hpp"
#include "RenderClass.hpp"
#include "Systems.hpp"

int main(void)
{
    RenderClass render(1280, 720, "R-Type", 120);
    ecs::RenderSystem renderSystem;
    std::vector<std::shared_ptr<ecs::Entity>> entities;
    sf::Texture texture;

    texture.loadFromFile("../assets/r-typesheet1.gif");

    auto player = std::make_shared<ecs::Entity>(0);
    player->addComponent(std::make_unique<ecs::PlayerComponent>("Samy"));
    player->addComponent(std::make_unique<ecs::PositionComponent>(10, 10));
    player->addComponent(std::make_unique<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, texture));

    auto enemy = std::make_shared<ecs::Entity>(1);
    enemy->addComponent(std::make_unique<ecs::PlayerComponent>("Arnaud"));
    enemy->addComponent(std::make_unique<ecs::PositionComponent>(100, 5));
    enemy->addComponent(std::make_unique<ecs::RenderComponent>(
        ecs::ObjectType::RECTANGLE, texture));

    entities.push_back(player);
    entities.push_back(enemy);

    while (render.getWindow().isOpen()) {
        render.getWindow().clear();
        renderSystem.update(entities, &render.getWindow(), 0);
        render.getWindow().display();
    }
}
