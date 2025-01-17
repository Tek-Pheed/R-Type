/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Levels.hpp
*/
#ifndef R_TYPE_LEVELS_HPP
#define R_TYPE_LEVELS_HPP

namespace RType
{
    class GameInstance;

    class Levels {
      public:
        explicit Levels(GameInstance &game) : _game(game) {};

        void buildMainMenu();
        void buildSettingsMenu();
        void buildConnectToServerMenu();
        void buildLobby();

        GameInstance &_game;
        // private:
    };
} // namespace RType

#endif // R_TYPE_LEVELS_HPP
