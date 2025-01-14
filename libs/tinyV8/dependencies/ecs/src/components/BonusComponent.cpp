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

    BonusComponent::BonusComponent(size_t bonusID, const ecs::Bonus &bonus)
    {
        this->_bonusID = bonusID;
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

    void BonusComponent::setBonusID(size_t bonusID)
    {
        this->_bonusID = bonusID;
    }

    size_t BonusComponent::getBonusID()
    {
        return this->_bonusID;
    }
} // namespace ecs
