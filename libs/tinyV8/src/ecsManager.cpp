/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ecsManager
*/

#include "EngineECSManager.hpp"

size_t Engine::generateNewId(void)
{
    static size_t entityCounter = 0U;

    if (entityCounter >= SIZE_MAX - 2)
        entityCounter = 0;
    return (entityCounter++);
}
