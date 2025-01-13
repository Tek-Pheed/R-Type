/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClickableComponent.cpp
*/

#include "components/ClickableComponent.hpp"

namespace ecs
{
    ClickableComponent::ClickableComponent(ecs::ClickableType type)
        : _type(type)
    {
    }

    ClickableType ClickableComponent::getClickableType()
    {
        return this->_type;
    }

    void ClickableComponent::setClickableType(ecs::ClickableType type)
    {
        this->_type = type;
    }
} // namespace ecs