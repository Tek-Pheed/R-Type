/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** RenderComponent
*/

#include "Components.hpp"

namespace ecs
{
    RenderComponent::RenderComponent(ObjectType type)
    {
        this->_type = type;
    }

    ObjectType RenderComponent::getObjectType() const
    {
        return this->_type;
    }

    void RenderComponent::setObjectType(ObjectType type)
    {
        this->_type = type;
    }
} // namespace ecs