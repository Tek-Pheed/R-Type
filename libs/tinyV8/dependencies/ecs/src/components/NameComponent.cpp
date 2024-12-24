/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NameComponent.cpp
*/

#include "Components.hpp"

namespace ecs
{
    NameComponent::NameComponent(const std::string &name)
    {
        this->_name = name;
    }

    std::string NameComponent::getName() const
    {
        return this->_name;
    }

    void NameComponent::setName(const std::string &name)
    {
        this->_name = name;
    }
} // namespace ecs