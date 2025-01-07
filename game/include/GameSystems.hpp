/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems.hpp
*/

#ifndef R_TYPE_SYSTEMS_HPP
#define R_TYPE_SYSTEMS_HPP

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.hpp"
#include "Systems.hpp"

namespace RType
{
    size_t getNewId();

    class GameInstance;

    namespace GameSystems
    {
        class RenderSystem : public ecs::ISystem<GameInstance> {
          public:
            void initSystem(GameInstance &gameRef) override;
            void update(
                std::vector<ecs::Entity> &entity, float deltaTime) override;

          protected:
            GameInstance *_game;
        };

        class PositionSystem : public ecs::ISystem<GameInstance> {
          public:
            void initSystem(GameInstance &gameRef) override;
            void update(
                std::vector<ecs::Entity> &entity, float deltaTime) override;

          protected:
            GameInstance *_game;
        };

        class BulletSystem : public ecs::ISystem<GameInstance> {
          public:
            void initSystem(GameInstance &gameRef) override;
            void update(
                std::vector<ecs::Entity> &entity, float deltaTime) override;

          protected:
            GameInstance *_game;
        };

        class BackgroundSystem : public ecs::ISystem<GameInstance> {
          public:
            void initSystem(GameInstance &gameRef) override;
            void update(
                std::vector<ecs::Entity> &entity, float deltaTime) override;

          protected:
            GameInstance *_game;
        };

        class HealthSystem : public ecs::ISystem<GameInstance> {
          public:
            void initSystem(GameInstance &gameRef) override;
            void update(
                std::vector<ecs::Entity> &entity, float deltaTime) override;

          protected:
            GameInstance *_game;
        };

        class HitboxSystem : public ecs::ISystem<GameInstance> {
          public:
            void initSystem(GameInstance &gameRef) override;
            void update(
                std::vector<ecs::Entity> &entity, float deltaTime) override;
            void BulletCollision(ecs::Entity &bullet);
            void EnemyCollision(ecs::Entity &enemy, float deltaTime);

          protected:
            GameInstance *_game;
        };

    } // namespace GameSystems

} // namespace RType

#endif // R_TYPE_SYSTEMS_HPP
