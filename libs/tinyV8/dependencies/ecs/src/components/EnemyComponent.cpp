/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** EnemyComponent
*/

#include "Components.hpp"

ecs::EnemyComponent::EnemyComponent(const std::string &name)
{
    this->_name = name;
}

std::string ecs::EnemyComponent::getName() const
{
    return this->_name;
}

void ecs::EnemyComponent::setName(const std::string &name)
{
    this->_name = name;
}
