/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** VelocityComponent
*/

#include "Components.hpp"

namespace ecs
{
    VelocityComponent::VelocityComponent(float vx, float vy)
    {
        this->_vx = vx;
        this->_vy = vy;
    }

    float VelocityComponent::getVx()
    {
        return this->_vx;
    }

    float VelocityComponent::getVy()
    {
        return this->_vy;
    }

    void VelocityComponent::setVx(float vx)
    {
        this->_vx = vx;
    }

    void VelocityComponent::setVy(float vy)
    {
        this->_vy = vy;
    }
} // namespace ecs
