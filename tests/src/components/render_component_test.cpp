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

Test(components, render_constructor)
{
    ecs::RenderComponent render(ecs::ObjectType::RECTANGLE);

    cr_assert_eq(render.getType(), ecs::ObjectType::RECTANGLE, "Expected type Rectangle, but got %d",
        render.getType());
}

Test(components, render_getter)
{
    ecs::RenderComponent render(ecs::ObjectType::SPRITE);

    cr_assert_eq(render.getType(), ecs::ObjectType::SPRITE, "Expected type Sprite, but got %d",
        render.getType());
}

Test(components, render_setter)
{
    ecs::RenderComponent render(ecs::ObjectType::SPRITE);

    render.setType(ecs::ObjectType::CIRCLE);

    cr_assert_eq(render.getType(), ecs::ObjectType::CIRCLE, "Expected type Circle, but got %d",
        render.getType());
}
