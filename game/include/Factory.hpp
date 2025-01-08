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
    static constexpr const char *DEFAULT_PLAYER_NAME = "Anonymous_Player";
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
        ecs::Entity &buildPlayer(bool isLocalPlayer, size_t id,
            const std::string &name = DEFAULT_PLAYER_NAME);
        ecs::Entity &buildBackground();
        void buildBulletFromPlayer(size_t playerID);
        void buildBulletFromEnemy(size_t enemyID);
        void buildBulletFromBoss(size_t bossID);
        ecs::Entity &buildEnemy(
            size_t id, float posX, float posY, float health = 100.0f);
        ecs::Entity &buildEnemyShooter(
            size_t id, float posX, float posY, float health = 100.0f);
        ecs::Entity &buildMusic(sf::SoundBuffer &soundBuffer);

      private:
        GameInstance &_game;
    };
} // namespace RType

#endif // R_TYPE_FACTORY_HPP
