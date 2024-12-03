/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include "Components.hpp"
#include "Entity.hpp"

int main(void)
{
    auto player = std::make_unique<ecs::Entity>(0);
    player->addComponent(std::make_unique<ecs::PositionComponent>(0, 0));
    player->addComponent(std::make_unique<ecs::PlayerComponent>("client"));

    /*std::cout << player->getComponent<ecs::PositionComponent>()->getX()
              << std::endl;*/

    auto pX = player->getComponent<ecs::PositionComponent>();

    return (0);
}
