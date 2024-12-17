/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PositionComponent.cpp
*/

#include "Components.hpp"

ecs::PositionComponent::PositionComponent(float x, float y)
{
    this->_x = x;
    this->_y = y;
    this->_oldX = x;
    this->_oldY = y;
}

float ecs::PositionComponent::getX()
{
    return this->_x;
}

float ecs::PositionComponent::getY()
{
    return this->_y;
}

float ecs::PositionComponent::getOldX()
{
    return this->_oldX;
}

float ecs::PositionComponent::getOldY()
{
    return this->_oldY;
}

void ecs::PositionComponent::setX(float x)
{
    this->setOldX(this->_x);
    this->_x = x;
}

void ecs::PositionComponent::setY(float y)
{
    this->setOldY(this->_y);
    this->_y = y;
}

void ecs::PositionComponent::setOldX(float oldX)
{
    this->_oldX = oldX;
}

void ecs::PositionComponent::setOldY(float oldY)
{
    this->_oldY = oldY;
}