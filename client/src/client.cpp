/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include "Entity.hpp"

int main(void)
{
    ecs::Entity entity(0);

    entity.addComponent(std::make_unique<ecs::PlayerComponent>("Samy"));
    entity.addComponent(std::make_unique<ecs::PositionComponent>(0, 0));
    entity.addComponent(std::make_unique<ecs::HealthComponent>());

    std::cout << entity.getComponent<ecs::PositionComponent>()->getX()
              << std::endl;

    return (0);
}
