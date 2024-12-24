/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems.hpp
*/
#ifndef R_TYPE_SYSTEMS_HPP
#define R_TYPE_SYSTEMS_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Entity.hpp"

namespace ecs
{
    class ISystem {
      public:
        virtual void update(std::vector<ecs::Entity> &entity,
            sf::RenderWindow &window, float deltaTime, bool isServer) = 0;
    };

    class RenderSystem : public ISystem {
      public:
        void update(std::vector<ecs::Entity> &entity, sf::RenderWindow &window,
            float deltaTime, bool isServer) override;
    };

    class PositionSystem : public ISystem {
      public:
        void update(std::vector<ecs::Entity> &entities,
            sf::RenderWindow &window, float deltaTime, bool isServer) override;
    };

    class BulletSystem : public ISystem {
      public:
        void update(std::vector<ecs::Entity> &entities,
            sf::RenderWindow &window, float deltaTime, bool isServer) override;
    };
} // namespace ecs

#endif // R_TYPE_SYSTEMS_HPP
