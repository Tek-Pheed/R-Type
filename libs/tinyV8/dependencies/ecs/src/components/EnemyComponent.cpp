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
        : _enemyID(enemyID), _type(type), _wave(0)
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

    int EnemyComponent::getWave() const
    {
        return (_wave);
    }

    void EnemyComponent::setWave(int wave)
    {
        _wave = wave;
    }

} // namespace ecs
