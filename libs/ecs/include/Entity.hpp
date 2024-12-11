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
        explicit Entity(std::size_t id);
        ~Entity();

        template <typename componentType>
        void addComponent(std::shared_ptr<componentType> component)
        {
            this->_components[typeid(componentType)] = std::move(component);
        }

        template <typename componentType>
        std::shared_ptr<componentType> getComponent()
        {
            auto index = this->_components.find(typeid(componentType));
            if (index != this->_components.end()) {
                return std::static_pointer_cast<componentType>(index->second);
            }
            return nullptr;
        }

        std::vector<std::type_index> getComponents()
        {
            std::vector<std::type_index> types;

            types.reserve(_components.size());
            for (const auto &t : _components) {
                types.emplace_back(t.first);
            }
            return (types);
        }

        template <typename componentType> void deleteComponent()
        {
            this->_components.erase(typeid(componentType));
        }

        std::size_t getID() const;
        void setID(std::size_t id);

      private:
        std::size_t _id;
        std::unordered_map<std::type_index, std::shared_ptr<Component>>
            _components;
    };
} // namespace ecs

#endif // R_TYPE_ENTITY_HPP
