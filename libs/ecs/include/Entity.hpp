/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity.hpp
*/
#ifndef R_TYPE_ENTITY_HPP
#define R_TYPE_ENTITY_HPP

#include <iostream>
#include <map>
#include <typeindex>
#include <vector>
#include "Components.hpp"

namespace ecs
{
    class Entity {
      public:
        Entity(std::size_t id);
        ~Entity();

        template <typename componentType>
        void addComponent(std::unique_ptr<componentType> component);

        template <typename componentType>
        std::unique_ptr<componentType> getComponent();

      private:
        std::size_t _id;
        std::unordered_map<std::type_index, std::unique_ptr<Component>>
            _components;
    };
} // namespace ecs

#endif // R_TYPE_ENTITY_HPP
