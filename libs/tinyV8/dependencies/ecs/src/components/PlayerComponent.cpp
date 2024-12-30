/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PlayerComponent.cpp
*/

#include "Components.hpp"

namespace ecs
{
    PlayerComponent::PlayerComponent(size_t playerID) : _playerID(playerID)
    {
    }

    size_t PlayerComponent::getPlayerID() const
    {
        return (_playerID);
    }

    void PlayerComponent::setPlayerID(size_t playerID)
    {
        _playerID = playerID;
    }

} // namespace ecs