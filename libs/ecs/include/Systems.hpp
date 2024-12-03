/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems.hpp
*/
#ifndef R_TYPE_SYSTEMS_HPP
#define R_TYPE_SYSTEMS_HPP

#include <SFML/Graphics.hpp>

namespace ecs
{
    class ISystem {
      public:
        virtual void update(sf::Window *window, float deltaTime) = 0;
    };

    class VelocitySystem : public ISystem {
      public:
        VelocitySystem();
        void update(sf::Window *window, float deltaTime) override;
    };
} // namespace ecs

#endif // R_TYPE_SYSTEMS_HPP
