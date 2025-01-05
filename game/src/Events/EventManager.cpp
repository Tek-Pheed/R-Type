/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventManager
*/

#include "Events.hpp"

namespace RType {
    EventManager::EventManager(GameInstance *gameInstance) {
        this->_game = gameInstance;
    }
}