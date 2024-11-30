/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PlayerComponent.cpp
*/

#include "Components.hpp"

ecs::PlayerComponent::PlayerComponent(const std::string &name)
{
    this->_name = name;
}

std::string ecs::PlayerComponent::getName() const
{
    return this->_name;
}

void ecs::PlayerComponent::setName(const std::string &name)
{
    this->_name = name;
}
