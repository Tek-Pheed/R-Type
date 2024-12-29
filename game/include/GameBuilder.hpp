/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameBuilder
*/

#ifndef GAME_BUILDER_HPP
#define GAME_BUILDER_HPP

#include "Entity.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"

namespace RType
{
    ecs::Entity &buildPlayer(GameInstance &game, const std::string &levelName);
    ecs::Entity &buildBackground(GameInstance &game);

    void buildMainMenu(GameInstance &game);
    void createPersistentLevel(GameInstance &game);

} // namespace RType

#endif /* GAME_BUILDER_HPP */