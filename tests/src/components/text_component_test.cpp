/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** text_component_test
*/

#include <dlfcn.h>
#include <iostream>
#include <criterion/criterion.h>
#include "Components.hpp"

Test(components, text_default_constructor)
{
    ecs::TextComponent text;

    cr_assert_eq(text.getText(), "",
        "Expected value to be '', but got %d", text.getText());
}

Test(components, text_constructor)
{
    ecs::TextComponent text;
    text.setText("Hello World");

    cr_assert_eq(text.getText(), "Hello World",
        "Expected value to be 'Hello World', but got '%s'", text.getText().c_str());
}

Test(components, text_getter)
{
    ecs::TextComponent text;
    text.setText("Hello World");

    cr_assert_eq(text.getText(), "Hello World",
        "Expected value to be 'Hello World', but got '%s'", text.getText().c_str());
}
