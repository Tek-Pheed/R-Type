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
#include <memory>
#include <thread>
#include <vector>
#include "RenderClass.hpp"

void client::add_entity(ecs::Entity &entity)
{
    _entities.push_back(entity);
}

std::vector<ecs::Entity> &client::get_entities()
{
    return _entities;
}

int client::get_id()
{
    return _id;
}

void create_player(client &client, RenderClass &render)
{
    auto &player = client.getLocalPlayer();
    sf::Sprite playerSP(render.getPlayerTexture());

    player = ecs::Entity((std::size_t) client.get_id());
    player.addComponent(std::make_shared<ecs::PlayerComponent>());
    player.addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player.addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    player.addComponent(
        std::make_shared<ecs::RenderComponent>(ecs::ObjectType::SPRITE));
    playerSP.setTextureRect(sf::Rect(66, 0, 33, 14));
    playerSP.setScale(sf::Vector2f(3, 3));
    playerSP.setPosition(100, 100);
    player.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(playerSP, 3, 3));
    client.add_entity(player);
}

int print_help()
{
    std::cout << "USAGE: ./client {IP} {PORT TCP} {PORT UDP}" << std::endl;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 4)
        return print_help();
    if (!atoi(argv[1]) || !atoi(argv[2]))
        return print_help();
    int portTCP = atoi(argv[2]);
    int portUDP = atoi(argv[3]);
    System::Network::initNetwork();
    std::cout << "Connecting to: " << argv[1] << " on port " << portTCP << " "
              << portUDP << std::endl;
    RenderClass render(1280, 720, "R-Type", 120);
    client client(render);

    client.create_connection(argv[1], portTCP, portUDP);
    std::thread(&client::receive_message, &client).detach();
    std::cout << "Client connected" << std::endl;

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/sprites/r-typesheet42.gif");

    render.setPlayerTexture(playerTexture);

    create_player(client, render);

    render.renderWindow(client);
}
