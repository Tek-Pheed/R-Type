/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventManager
*/

#include <memory>
#include "Entity.hpp"
#include "Events.hpp"

namespace RType
{
    EventManager::EventManager(GameInstance &gameInstance, Factory &factory)
        : _factory(factory), _game(gameInstance),
          _entity(std::make_unique<ecs::Entity>(ecs::Entity(0)))
    {
    }
} // namespace RType
