/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** entity_test.cpp
*/

#include <dlfcn.h>
#include <iostream>
#include <criterion/criterion.h>
#include "Entity.hpp"

Test(entity, basic_entity)
{
    auto player = std::make_unique<ecs::Entity>(0);
    player->addComponent(std::make_unique<ecs::PositionComponent>(10, 10));
    player->addComponent(std::make_unique<ecs::PlayerComponent>("client"));

    auto player_pos = player->getComponent<ecs::PositionComponent>();
    auto player_name = player->getComponent<ecs::PlayerComponent>();

    cr_assert_eq(player_pos.get()->getX(), 10,
        "Expected value to be 10, but got %d", player_pos.get()->getX());
    cr_assert_eq(player_name.get()->getName(), "client",
        "Expected value to be 'client', but got %s",
        player_name.get()->getName());
}