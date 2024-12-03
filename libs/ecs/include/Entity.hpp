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
#include <memory>
#include <typeindex>
#include <vector>
#include "Components.hpp"
#include <unordered_map>

namespace ecs
{
    class Entity {
      public:
        Entity(std::size_t id);
        ~Entity();

        template <typename componentType>
        void addComponent(std::unique_ptr<componentType> component)
        {
            this->_components[typeid(componentType)] = std::move(component);
        }

        template <typename componentType>
        std::unique_ptr<componentType> getComponent()
        {
            auto index = this->_components.find(typeid(componentType));
            if (index != this->_components.end()) {
                std::unique_ptr<componentType> ptr(
                    static_cast<componentType *>(index->second.release()));
                return ptr;
            }
            return nullptr;
        }

      private:
        std::size_t _id;
        std::unordered_map<std::type_index, std::unique_ptr<Component>>
            _components;
    };
} // namespace ecs

#endif // R_TYPE_ENTITY_HPP
