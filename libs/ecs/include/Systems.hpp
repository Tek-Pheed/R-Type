/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems.hpp
*/
#ifndef R_TYPE_SYSTEMS_HPP
#define R_TYPE_SYSTEMS_HPP

#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

namespace ecs
{
    class ISystem {
      public:
        virtual void update(std::vector<std::unique_ptr<ecs::Entity>> &entities, sf::RenderWindow *window, float deltaTime) = 0;

    };

    class PositionSystem : public ISystem {
      public:

        void update(std::vector<std::unique_ptr<ecs::Entity>> &entities, sf::RenderWindow *window, float deltaTime) override;

    };
} // namespace ecs

#endif // R_TYPE_SYSTEMS_HPP
