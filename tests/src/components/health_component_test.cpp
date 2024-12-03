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

Test(components, health_default_constructor)
{
    ecs::HealthComponent health;

    cr_assert_eq(health.getHealth(), 100,
        "Expected value to be 100, but got %d", health.getHealth());
}

Test(components, health_constructor)
{
    ecs::HealthComponent health(200);

    cr_assert_eq(health.getHealth(), 200,
        "Expected value to be 200, but got %d", health.getHealth());
}

Test(components, health_getter)
{
    ecs::HealthComponent health;

    cr_assert_eq(health.getHealth(), 100,
        "Expected value to be 100, but got %d", health.getHealth());
}

Test(components, health_setter)
{
    ecs::HealthComponent health;
    health.setHealth(80);

    cr_assert_eq(health.getHealth(), 80, "Expected value to be 80, but got %d",
        health.getHealth());
}