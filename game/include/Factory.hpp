/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Factory.hpp
*/
#ifndef R_TYPE_FACTORY_HPP
#define R_TYPE_FACTORY_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Game.hpp"

namespace RType
{
    class Factory {
      public:
        explicit Factory(GameInstance *game);

        ecs::Entity &buildButton(sf::Vector2f position, sf::Vector2f size,
            sf::Color fillColor = sf::Color::White,
            sf::Color outlineColor = sf::Color::Blue,
            const std::string &str = "", unsigned int characterSize = 40,
            sf::Color textColor = sf::Color::Black);
        ecs::Entity &buildPlayer(bool isLocalPlayer, size_t id);
        ecs::Entity &buildBackground();
        void buildBulletFromPlayer(size_t playerID);

      private:
        GameInstance *_game;
    };
} // namespace RType

#endif // R_TYPE_FACTORY_HPP
