/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** BossComponent
*/

#include "Components.hpp"

namespace ecs
{
    BossComponent::BossComponent(size_t bossID) : _bossID(bossID)
    {
    }

    size_t BossComponent::getBossID() const
    {
        return (_bossID);
    }

    void BossComponent::setBossID(size_t bossID)
    {
        _bossID = bossID;
    }

} // namespace ecs