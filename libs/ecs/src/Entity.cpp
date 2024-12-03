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

template <typename componentType>
void ecs::Entity::addComponent(std::unique_ptr<componentType> component)
{
    this->_components[typeid(this->_id)] = component;
}

template <typename componentType>
std::unique_ptr<componentType> ecs::Entity::getComponent()
{
    auto index = this->_components.find(typeid(componentType));
    if (index != this->_components.end()) {
        return std::static_pointer_cast<componentType>(index->second);
    }
    return nullptr;
}