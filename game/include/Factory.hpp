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
        void buildBulletFromBoss(
            size_t bossId, float velx = 0.0f, float vely = 0.0f);
        ecs::Entity &buildEnemy(size_t id, float posX, float posY,
            float health = 100.0f, int wave = 0, float velocityX = -200.0f,
            float velocityY = 0.0f);
        ecs::Entity &buildText(size_t id, float posX, float posY,
            std::string &content, sf::Color color, unsigned int characterSize);
        ecs::Entity &buildMusic(sf::SoundBuffer &soundBuffer, std::string str);
        ecs::Entity &buildSoundEffect(
            sf::SoundBuffer &soundBuffer, std::string str, float volume);
        ecs::Entity &buildEnemyShooter(size_t id, float posX, float posY,
            float health = 100.0f, int wave = 0, float velocityX = -200.0f,
            float velocityY = 0.0f);
        ecs::Entity &buildBoss(size_t id, float posX, float posY,
            float health = 5000.0f, int wave = 0);
        ecs::Entity &buildAIPlayer(sf::Vector2f velocity,
            const std::string &name, std::size_t skinID);
        ecs::Entity &buildExplosionEnemy(float posX, float posY);
        ecs::Entity &buildExplosionPlayer(float posX, float posY);
        ecs::Entity &buildBonus(
            size_t id, float posX, float posY, ecs::Bonus bonus);
        ecs::Entity &buildInput(const std::string &str, int buttonID);

      private:
        GameInstance &_game;
    };
} // namespace RType

#endif // R_TYPE_FACTORY_HPP
