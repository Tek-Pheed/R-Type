/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** boss_component_test
*/

#include <dlfcn.h>
#include <iostream>
#include <criterion/criterion.h>
#include "Components.hpp"

Test(components, boss_constructor)
{
    ecs::BossComponent boss("SamyTheBoss");
    cr_assert_eq(boss.getName(), "SamyTheBoss",
        "Expected value to be 'boss', but got %d", boss.getName());
}

Test(components, boss_getter)
{
    ecs::BossComponent boss("chehSamyTheBoss");
    cr_assert_eq(boss.getName(), "chehSamyTheBoss",
        "Expected value to be 'SamyTheBoss', but got %d", boss.getName());
}

Test(components, boss_setter)
{
    ecs::BossComponent boss("SamyTheBoss");
    boss.setName("LucasTheBoss");
    cr_assert_eq(boss.getName(), "LucasTheBoss",
        "Expected value to be 'LucasTheBoss', but got %d", boss.getName());
}
