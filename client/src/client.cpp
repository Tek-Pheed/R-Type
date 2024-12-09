/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include "client.hpp"
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
    client c;

    // To be set to user input later
    c.create_connection("127.0.0.1", 8081, 8082);
    // Set to non-blocking
    c.receive_message();

    texture.loadFromFile("../assets/sprites/r-typesheet1.gif");

    auto player = std::make_shared<ecs::Entity>(0);
    player->addComponent(std::make_shared<ecs::PlayerComponent>("Samy"));
    player->addComponent(std::make_shared<ecs::PositionComponent>(10, 10));
    player->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, texture));

    auto enemy = std::make_shared<ecs::Entity>(1);
    enemy->addComponent(std::make_shared<ecs::PlayerComponent>("Arnaud"));
    enemy->addComponent(std::make_shared<ecs::PositionComponent>(100, 5));
    enemy->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::RECTANGLE, texture));

    entities.push_back(player);
    entities.push_back(enemy);

    while (render.getWindow().isOpen()) {
        render.getWindow().clear();
        renderSystem.update(entities, &render.getWindow(), 0);
        render.getWindow().display();
    }
    return (0);
}
