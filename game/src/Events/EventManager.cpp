/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventManager
*/

#include "Events.hpp"

namespace RType {
    EventManager::EventManager(GameInstance *gameInstance): _game(gameInstance), _entity(new ecs::Entity(0)) {
    }
}
