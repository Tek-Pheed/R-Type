/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** VelocityComponent
*/

#include "Components.hpp"

ecs::VelocityComponent::VelocityComponent(float vx, float vy)
{
    this->_vx = vx;
    this->_vy = vy;
}

float ecs::VelocityComponent::getVx()
{
    return this->_vx;
}

float ecs::VelocityComponent::getVy()
{
    return this->_vy;
}

void ecs::VelocityComponent::setVx(float vx)
{
    this->_vx = vx;
}

void ecs::VelocityComponent::setVy(float vy)
{
    this->_vy = vy;
}
