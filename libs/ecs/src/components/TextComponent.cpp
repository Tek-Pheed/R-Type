/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** TextComponent
*/

#include "Components.hpp"

ecs::TextComponent::TextComponent()
{
}

void ecs::TextComponent::setText(const std::string &text)
{
    this->_text = text;
}

std::string ecs::TextComponent::getText() const
{
    return this->_text;
}
