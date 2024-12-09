/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** enemy_component_test
*/

#include <dlfcn.h>
#include <iostream>
#include <criterion/criterion.h>
#include "Components.hpp"

Test(components, enemy_constructor)
{
    ecs::EnemyComponent enemy("SamyTheEnemy");
    cr_assert_eq(enemy.getName(), "SamyTheEnemy",
        "Expected value to be 'enemy', but got %d", enemy.getName());
}

Test(components, enemy_getter)
{
    ecs::EnemyComponent enemy("chehSamyTheEnemy");
    cr_assert_eq(enemy.getName(), "chehSamyTheEnemy",
        "Expected value to be 'SamyTheEnemy', but got %d", enemy.getName());
}

Test(components, enemy_setter)
{
    ecs::EnemyComponent enemy("SamyTheEnemy");
    enemy.setName("LucasTheEnemy");
    cr_assert_eq(enemy.getName(), "LucasTheEnemy",
        "Expected value to be 'LucasTheEnemy', but got %d", enemy.getName());
}
