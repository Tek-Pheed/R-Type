/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** EnemyComponent
*/

#include "Components.hpp"

namespace ecs
{
    EnemyComponent::EnemyComponent(size_t enemyID, size_t type)
        : _enemyID(enemyID), _type(type)
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

    size_t EnemyComponent::getType() const
    {
        return _type;
    }
    void EnemyComponent::setType(size_t type)
    {
        _type = type;
    }

} // namespace ecs
