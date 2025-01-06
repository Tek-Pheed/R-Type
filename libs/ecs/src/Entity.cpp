/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity.cpp
*/

#include "Entity.hpp"

ecs::Entity::Entity(std::size_t id)
{
    this->_id = id;
}

ecs::Entity::~Entity()
{
}

std::size_t ecs::Entity::getId() const
{
    return _id;
}
