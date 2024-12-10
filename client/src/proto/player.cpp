/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#include "Entity.hpp"
#include "client.hpp"
#include "protocol.hpp"

void create_new_player(std::vector<std::string> &tokens)
{
    const int id = std::stoi(tokens[0]);
    const int x = std::stoi(tokens[1]);
    const int y = std::stoi(tokens[2]);
    sf::Texture playerTexture;

    playerTexture.loadFromFile("../../assets/sprites/r-typesheet42.gif");

    auto player = std::make_shared<ecs::Entity>(id);
    player->addComponent(std::make_shared<ecs::PlayerComponent>(tokens[0]));
    player->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));

    player->addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::ObjectType::SPRITE, playerTexture));
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
            break;
        }
        case P_DEAD: {
            break;
        }
        case P_SHOOT: {
            break;
        }
        case P_DMG: {
            break;
        }
        case P_DISCONN: {
            break;
        }
        default: {
            break;
        }
    }
}
