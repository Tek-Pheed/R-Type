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
}

float ecs::PositionComponent::getX()
{
    return this->_x;
}

float ecs::PositionComponent::getY()
{
    return this->_y;
}

void ecs::PositionComponent::setX(float x)
{
    this->_x = x;
}

void ecs::PositionComponent::setY(float y)
{
    this->_y = y;
}