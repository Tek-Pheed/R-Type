/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#ifndef ECS_SYSTEMS_HPP_
#define ECS_SYSTEMS_HPP_

#include <vector>
#include "Entity.hpp"

namespace ecs
{
    template <typename GameClass> class ISystem {
      public:
        virtual void initSystem(GameClass &gameRef) = 0;
        virtual void update(
            std::vector<ecs::Entity> &entity, float deltaTime) = 0;
        virtual ~ISystem() = default;
    };
} // namespace ecs

#endif // !ECS_SYSTEMS_HPP_
