/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PositionComponent.cpp
*/

#include "Components.hpp"

ecs::PositionComponent::PositionComponent(int x, int y)
{
    this->_x = x;
    this->_y = y;
}

int ecs::PositionComponent::getX()
{
    return this->_x;
}

int ecs::PositionComponent::getY()
{
    return this->_y;
}

void ecs::PositionComponent::setX(int x)
{
    this->_x = x;
}

void ecs::PositionComponent::setY(int y)
{
    this->_y = y;
}