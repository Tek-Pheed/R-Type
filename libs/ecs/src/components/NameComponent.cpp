/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NameComponent.cpp
*/

#include "Components.hpp"

ecs::NameComponent::NameComponent(const std::string &name)
{
    this->_name = name;
}

std::string ecs::NameComponent::getName() const
{
    return this->_name;
}

void ecs::NameComponent::setName(const std::string &name)
{
    this->_name = name;
}
