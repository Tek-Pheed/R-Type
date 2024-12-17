/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** RenderComponent
*/

#include "Components.hpp"

ecs::RenderComponent::RenderComponent(ObjectType type)
{
    this->_type = type;
}

ecs::ObjectType ecs::RenderComponent::getObjectType() const
{
    return this->_type;
}

void ecs::RenderComponent::setObjectType(ecs::ObjectType type)
{
    this->_type = type;
}