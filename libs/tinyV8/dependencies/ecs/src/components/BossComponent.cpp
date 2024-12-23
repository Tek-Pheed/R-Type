/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** BossComponent
*/

#include "Components.hpp"

ecs::BossComponent::BossComponent(const std::string &name)
{
    this->_name = name;
}

std::string ecs::BossComponent::getName() const
{
    return this->_name;
}

void ecs::BossComponent::setName(const std::string &name)
{
    this->_name = name;
}
