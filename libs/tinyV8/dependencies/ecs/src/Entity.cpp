/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity.cpp
*/

#include "Entity.hpp"

namespace ecs
{
    Entity::Entity(std::size_t id)
    {
        this->_id = id;
    }

    Entity::~Entity()
    {
    }

    std::size_t Entity::getID() const
    {
        return this->_id;
    }

    void Entity::setID(std::size_t id)
    {
        this->_id = id;
    }
} // namespace ecs