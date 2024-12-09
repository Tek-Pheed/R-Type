/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include "client.hpp"
#include <memory>
#include <thread>
#include <vector>
#include <ctime>
#include "Entity.hpp"
#include "RenderClass.hpp"
#include "Systems.hpp"

int main(void)
{
    RenderClass render(1280, 720, "R-Type", 120);
    std::vector<std::shared_ptr<ecs::Entity>> entities;
    client c;

    // To be set to user input later
    c.create_connection("127.0.0.1", 8081, 8082);
    std::thread(&client::receive_message, &c).detach();
    std::cout << "Client connected" << std::endl;

    sf::Texture player1texture;
    srand(static_cast<unsigned int>(time(0)));

    player1texture.loadFromFile("./assets/sprites/r-typesheet42.gif");

    auto player = std::make_shared<ecs::Entity>(rand());
    player->addComponent(std::make_shared<ecs::PlayerComponent>("Samy"));
    player->addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    player->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, player1texture));
    player->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(sf::Rect (66, 0, 33, 14));
    player->getComponent<ecs::RenderComponent>()->getSprite()->setScale(sf::Vector2f (3, 3));

    entities.push_back(player);
    render.renderWindow(entities, player);
}
