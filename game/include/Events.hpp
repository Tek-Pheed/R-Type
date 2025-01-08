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

#include <memory>
#include <Entity.hpp>
#include <memory>
#include <vector>
#include "Game.hpp"

namespace RType
{
    class EventManager {
      public:
        explicit EventManager(GameInstance &gameInstance, Factory &factory);

        void keyPressed(sf::Event &event);
        void keyReleased(sf::Event &event);
        void mouseClicked();

      protected:
        Factory &_factory;
        GameInstance &_game;
    };

} // namespace RType

#endif /* !EVENTCLASS_HPP_ */
