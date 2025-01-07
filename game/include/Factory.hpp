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
#include "components/ClickableComponent.hpp"

namespace RType
{
  class GameInstance;
    class Factory {
      public:
        explicit Factory(GameInstance &game);

        ecs::Entity &buildButton(sf::Vector2f position, sf::Vector2f size,
            sf::Color fillColor = sf::Color::White,
            sf::Color outlineColor = sf::Color::Blue,
            const std::string &str = "", unsigned int characterSize = 40,
            sf::Color textColor = sf::Color::Black,
            ecs::ClickableType type = ecs::ClickableType::LAUNCH);
        ecs::Entity &buildPlayer(
            bool isLocalPlayer, size_t id, const std::string &name);
        ecs::Entity &buildBackground();
        void buildBulletFromPlayer(size_t playerID);
        ecs::Entity &buildEnemy(
            size_t id, float posX, float posY, float health = 100.0f);

      private:
        GameInstance &_game;
    };
} // namespace RType

#endif // R_TYPE_FACTORY_HPP
