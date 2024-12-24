/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#ifndef ECS_SYSTEMS_HPP_
#define ECS_SYSTEMS_HPP_

#include <any>
#include <vector>
#include "Entity.hpp"
#include <initializer_list>

namespace ecs
{
    class ISystem {
      public:
        virtual void update(std::vector<ecs::Entity> &entity, float deltaTime,
            bool isServer) = 0;
    };
} // namespace ecs

#endif // !ECS_SYSTEMS_HPP_
