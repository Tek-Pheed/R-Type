/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BonusComponent.cpp
*/

#include "Components.hpp"

namespace ecs
{
    BonusComponent::BonusComponent(const ecs::Bonus &bonus)
    {
        this->_bonus = bonus;
    }

    Bonus BonusComponent::getBonus()
    {
        return this->_bonus;
    }

    void BonusComponent::setBonus(const ecs::Bonus &bonus)
    {
        this->_bonus = bonus;
    }
} // namespace ecs
