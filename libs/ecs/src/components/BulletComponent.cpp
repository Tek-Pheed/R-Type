/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** BulletComponent
*/

#include "Components.hpp"

ecs::BulletComponent::BulletComponent()
{
    this->_isFromPlayer = false;
}

ecs::BulletComponent::BulletComponent(bool isFromPlayer)
{
    this->_isFromPlayer = isFromPlayer;
}

std::string ecs::BulletComponent::getIsFromPlayer()
{
    return this->_isFromPlayer;
}

void ecs::BulletComponent::setIsFromPlayer(bool isFromPlayer)
{
    this->_isFromPlayer = isFromPlayer;
}
