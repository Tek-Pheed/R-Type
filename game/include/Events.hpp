/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** Event
*/

#ifndef EVENTCLASS_HPP_
#define EVENTCLASS_HPP_

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <Entity.hpp>
#include <memory>
#include <vector>
#include "Game.hpp"

using namespace RType;

class EventManager {
  public:
    explicit EventManager(GameInstance &gameInstance);

    void keyPressed(sf::Event &event);
    void keyReleased(sf::Event &event);
    void mouseClicked();
    void handleNumberOfPlayerButton(ecs::Entity &entity, bool isHost);
    void handleDifficultyButton(ecs::Entity &entity, bool isHost);
    void handleBonusButton(ecs::Entity &entity, bool isHost);
    void handleLevelButton(ecs::Entity &entity, bool isHost);

  protected:
    GameInstance &_game;
};

#endif /* !EVENTCLASS_HPP_ */
