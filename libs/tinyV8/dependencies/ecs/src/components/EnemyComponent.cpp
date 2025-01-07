/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** EnemyComponent
*/

#include "Components.hpp"

namespace ecs
{
    EnemyComponent::EnemyComponent(size_t enemyID) : _enemyID(enemyID)
    {
    }

    size_t EnemyComponent::getEnemyID() const
    {
        return (_enemyID);
    }

    void EnemyComponent::setEnemyID(size_t enemyID)
    {
        _enemyID = enemyID;
    }

} // namespace ecs
