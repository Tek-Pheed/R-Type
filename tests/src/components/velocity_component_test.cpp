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

Test(components, velocity_constructor)
{
    ecs::VelocityComponent velocity(10, 10);
    cr_assert_eq(velocity.getVx(), 10, "Expected value to be 10, but got %d",
        velocity.getVx());
    cr_assert_eq(velocity.getVy(), 10, "Expected value to be 10, but got %d",
        velocity.getVy());
}

Test(components, velocity_getter)
{
    ecs::VelocityComponent velocity(5, 5);
    cr_assert_eq(velocity.getVx(), 5, "Expected value to be 5, but got %d",
        velocity.getVx());
    cr_assert_eq(velocity.getVy(), 5, "Expected value to be 5, but got %d",
        velocity.getVy());
}

Test(components, velocity_setter)
{
    ecs::VelocityComponent velocity(5, 5);
    velocity.setVx(10);
    velocity.setVy(10);

    cr_assert_eq(velocity.getVx(), 10, "Expected value to be 10, but got %d",
        velocity.getVx());
    cr_assert_eq(velocity.getVy(), 10, "Expected value to be 10, but got %d",
        velocity.getVy());
}
