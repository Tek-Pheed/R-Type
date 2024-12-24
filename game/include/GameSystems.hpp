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
#include "SFML/Graphics/RenderWindow.hpp"
#include "Systems.hpp"
#include "Entity.hpp"

// TODO:
// A better solution then using raw pointer here would be
// to handle the ownership of the window in the game engine
// with a templated renderSubSystem storing the window
// then just pass the game engine in the system update

class RenderSystem : public ecs::ISystem {
  public:

    void initSystem(sf::RenderWindow *win);
    void update(std::vector<ecs::Entity> &entity,
        float deltaTime, bool isServer) override;

  protected:
    sf::RenderWindow *_win;
};

class PositionSystem : public ecs::ISystem {
  public:
    void initSystem(sf::RenderWindow *win);
    void update(std::vector<ecs::Entity> &entity,
        float deltaTime, bool isServer) override;

  protected:
    sf::RenderWindow *_win;
};

class BulletSystem : public ecs::ISystem {
  public:
    void initSystem(sf::RenderWindow *win);
    void update(std::vector<ecs::Entity> &entity,
        float deltaTime, bool isServer) override;

  protected:
    sf::RenderWindow *_win;
};

#endif // R_TYPE_SYSTEMS_HPP
