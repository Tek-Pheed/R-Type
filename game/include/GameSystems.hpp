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
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Window.hpp"
#include "Systems.hpp"

class Game;

class RenderSystem : public ecs::ISystem<Game> {
  public:
    void initSystem(Game &gameRef) override;
    void update(std::vector<ecs::Entity> &entity, float deltaTime) override;

  protected:
    Game *_game;
};

class PositionSystem : public ecs::ISystem<Game> {
  public:
    void initSystem(Game &gameRef) override;
    void update(std::vector<ecs::Entity> &entity, float deltaTime) override;

  protected:
    Game *_game;
};

class BulletSystem : public ecs::ISystem<Game> {
  public:
    void initSystem(Game &gameRef) override;
    void update(std::vector<ecs::Entity> &entity, float deltaTime) override;

  protected:
    Game *_game;
};

#endif // R_TYPE_SYSTEMS_HPP
