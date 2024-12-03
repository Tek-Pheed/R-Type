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

ecs::ObjectType ecs::RenderComponent::getType() const {
    return _type;
}

void ecs::RenderComponent::setType(ObjectType type) {
    _type = type;
}
