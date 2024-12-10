/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "client.hpp"
#include <ctime>
#include <memory>
#include <thread>
#include <vector>
#include "Entity.hpp"
#include "RenderClass.hpp"
#include "system_network.hpp"

void client::add_entity(std::shared_ptr<ecs::Entity> entity)
{
    _entities.push_back(entity);
}

std::vector<std::shared_ptr<ecs::Entity>> &client::get_entities()
{
    return _entities;
}

int client::get_id()
{
    return _id;
}

int main(void)
{
    client client;
    System::Network::initNetwork();

    // To be set to user input later
    client.create_connection("127.0.0.1", 8081, 8082);
    std::thread(&client::receive_message, &client).detach();
    std::cout << "Client connected" << std::endl;

    RenderClass render(1280, 720, "R-Type", 120);

    sf::Texture player1texture;
    srand(static_cast<unsigned int>(time(0)));

    player1texture.loadFromFile("assets/sprites/r-typesheet42.gif");

    auto player = std::make_shared<ecs::Entity>(client.get_id());
    player->addComponent(std::make_shared<ecs::PlayerComponent>("Samy"));
    player->addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));

    player->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, player1texture));

    player->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
        sf::Rect(66, 0, 33, 14));

    player->getComponent<ecs::RenderComponent>()->getSprite()->setScale(
        sf::Vector2f(3, 3));

    client.add_entity(player);
    render.renderWindow(player, client);
}
