/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PositionComponent.cpp
*/

#include "Components.hpp"

namespace ecs
{
    PositionComponent::PositionComponent(float x, float y)
    {
        this->_x = x;
        this->_y = y;
        this->_oldX = x;
        this->_oldY = y;
    }

    float PositionComponent::getX()
    {
        return this->_x;
    }

    float PositionComponent::getY()
    {
        return this->_y;
    }

    float PositionComponent::getOldX()
    {
        return this->_oldX;
    }

    float PositionComponent::getOldY()
    {
        return this->_oldY;
    }

    void PositionComponent::setX(float x)
    {
        this->setOldX(this->_x);
        this->_x = x;
    }

    void PositionComponent::setY(float y)
    {
        this->setOldY(this->_y);
        this->_y = y;
    }

    void PositionComponent::setOldX(float oldX)
    {
        this->_oldX = oldX;
    }

    void PositionComponent::setOldY(float oldY)
    {
        this->_oldY = oldY;
    }
} // namespace ecs