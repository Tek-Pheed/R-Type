/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** VelocityComponent
*/

#include "Components.hpp"

ecs::VelocityComponent::VelocityComponent(int vx, int vy)
{
    this->_vx = vx;
    this->_vy = vy;
}

int ecs::VelocityComponent::getVx()
{
    return this->_vx;
}

int ecs::VelocityComponent::getVy()
{
    return this->_vy;
}

void ecs::VelocityComponent::setVx(int vx)
{
    this->_vx = vx;
}

void ecs::VelocityComponent::setVy(int vy)
{
    this->_vy = vy;
}
