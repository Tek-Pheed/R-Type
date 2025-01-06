/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** BulletComponent
*/

#include "Components.hpp"

namespace ecs
{
    BulletComponent::BulletComponent(bool isFromPlayer)
    {
        this->_isFromPlayer = isFromPlayer;
    }

    bool BulletComponent::getIsFromPlayer()
    {
        return this->_isFromPlayer;
    }

    void BulletComponent::setIsFromPlayer(bool isFromPlayer)
    {
        this->_isFromPlayer = isFromPlayer;
    }
} // namespace ecs
