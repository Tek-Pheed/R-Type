/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** HealthComponent.cpp
*/

#include "Components.hpp"

namespace ecs
{
    HealthComponent::HealthComponent()
    {
        this->_health = 100;
        _oldHealth = 100;
    }

    HealthComponent::HealthComponent(int health)
    {
        this->_health = health;
    }

    int HealthComponent::getHealth()
    {
        return this->_health;
    }

    void HealthComponent::setHealth(int health)
    {
        _oldHealth = this->_health;
        this->_health = health;
    }

    int HealthComponent::getOldHealth()
    {
        return this->_oldHealth;
    }

    void HealthComponent::setOldHealth(int oldHealth)
    {
        this->_oldHealth = oldHealth;
    }
} // namespace ecs
