/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** HealthComponent.cpp
*/

#include "Components.hpp"

ecs::HealthComponent::HealthComponent()
{
    this->_health = 100;
}

ecs::HealthComponent::HealthComponent(int health)
{
    this->_health = health;
}

int ecs::HealthComponent::getHealth()
{
    return this->_health;
}

void ecs::HealthComponent::setHealth(int health)
{
    this->_health = health;
}