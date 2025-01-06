/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HitboxComponent
*/

#include "Components.hpp"

namespace ecs
{
    HitboxComponent::HitboxComponent(float height, float width)
        : _height(height), _width(width)
    {
    }

    float HitboxComponent::getHeight() const
    {
        return this->_height;
    }
    float HitboxComponent::getWidth() const
    {
        return this->_width;
    }

    void HitboxComponent::setHeight(float height)
    {
        this->_height = height;
    }
    void HitboxComponent::setWidth(float width)
    {
        this->_width = width;
    }

} // namespace ecs
