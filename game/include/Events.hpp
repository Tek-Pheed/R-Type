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

#include "Game.hpp"
#include <Entity.hpp>
#include <vector>

namespace RType {
    class EventManager {
        public:
            explicit EventManager(GameInstance *gameInstance);

            void keyPressed(sf::Event &event);
            void keyReleased(sf::Event &event);
            void mouseClicked();

        protected:
            GameInstance *_game;
            ecs::Entity *_entity;
    };

}

#endif /* !EVENTCLASS_HPP_ */
