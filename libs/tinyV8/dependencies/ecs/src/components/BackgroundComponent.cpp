/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** BackgroundComponent
*/

#include "Components.hpp"

namespace ecs
{
    BackgroundComponent::BackgroundComponent(float moveDelta)
        : _moveDelta(moveDelta)
    {
    }

    float BackgroundComponent::getClock(void) const
    {
        return (_deltaCounter);
    }

    float BackgroundComponent::setClock(float delta)
    {
        _deltaCounter = delta;
        return (_deltaCounter);
    }

    bool BackgroundComponent::isTimeToMove(void)
    {
        return (_deltaCounter >= _moveDelta);
    }

} // namespace ecs