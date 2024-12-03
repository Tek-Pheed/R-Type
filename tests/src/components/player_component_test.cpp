/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position_component_test.cpp
*/

#include <dlfcn.h>
#include <iostream>
#include <criterion/criterion.h>
#include "Components.hpp"

Test(components, player_constructor)
{
    ecs::PlayerComponent player("samy");

    cr_assert_eq(player.getName(), "samy",
        "Expected value to be 'samy', but got %d", player.getName());
}

Test(components, player_getter)
{
    ecs::PlayerComponent player("cheh");

    cr_assert_eq(player.getName(), "cheh",
        "Expected value to be 'cheh', but got %d", player.getName());
}

Test(components, player_setter)
{
    ecs::PlayerComponent player("samy");
    player.setName("Lucas");

    cr_assert_eq(player.getName(), "Lucas",
        "Expected value to be 'Lucas', but got %d", player.getName());
}