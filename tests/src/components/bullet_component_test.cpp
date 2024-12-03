/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** bullet_component_test
*/

#include <dlfcn.h>
#include <iostream>
#include <criterion/criterion.h>
#include "Components.hpp"

Test(components, bullet_constructor)
{
    ecs::BulletComponent bullet();
    cr_assert_eq(bullet.getIsFromPlayer(), false,
        "Expected value to be false, but got %d", bullet.getIsFromPlayer());
}

Test(components, bullet_getter)
{
    ecs::BulletComponent bullet(false);
    cr_assert_eq(bullet.getIsFromPlayer(), false,
        "Expected value to be false, but got %d", bullet.getIsFromPlayer());
}

Test(components, bullet_setter)
{
    ecs::BulletComponent bullet(false);
    bullet.setIsFromPlayer(true);
    cr_assert_eq(bullet.getIsFromPlayer(), true,
        "Expected value to be true, but got %d", bullet.getName());
}
