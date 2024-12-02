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

Test(components, position_constructor)
{
    ecs::PositionComponent position(10, 10);
    cr_assert_eq(position.getX(), 10, "Expected value to be 10, but got %d",
        position.getX());
    cr_assert_eq(position.getY(), 10, "Expected value to be 10, but got %d",
        position.getY());
}

Test(components, position_getter)
{
    ecs::PositionComponent position(0, 0);
    cr_assert_eq(position.getX(), 0, "Expected value to be 10, but got %d",
        position.getX());
    cr_assert_eq(position.getY(), 0, "Expected value to be 10, but got %d",
        position.getY());
}

Test(components, position_setter)
{
    ecs::PositionComponent position(0, 0);
    position.setX(10);
    position.setY(10);

    cr_assert_eq(position.getX(), 10, "Expected value to be 10, but got %d",
        position.getX());
    cr_assert_eq(position.getY(), 10, "Expected value to be 10, but got %d",
        position.getY());
}