/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** BulletComponent
*/

#include "Components.hpp"

namespace ecs
{
    BulletComponent::BulletComponent(bool isFromPlayer, size_t type, int team)
    {
        this->_isFromPlayer = isFromPlayer;
        _type = type;
        _team = team;
    }

    bool BulletComponent::getIsFromPlayer()
    {
        return this->_isFromPlayer;
    }

    void BulletComponent::setIsFromPlayer(bool isFromPlayer)
    {
        this->_isFromPlayer = isFromPlayer;
    }

    size_t BulletComponent::getType() const
    {
        return _type;
    }

    void BulletComponent::setType(size_t type)
    {
        _type = type;
    }

    int BulletComponent::getTeam() const
    {
        return _team;
    }

    void BulletComponent::setTeam(int team)
    {
        _team = team;
    }
} // namespace ecs
