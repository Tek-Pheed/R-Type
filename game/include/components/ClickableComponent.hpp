/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClickableComponent.hpp
*/
#ifndef R_TYPE_CLICKABLECOMPONENT_HPP
#define R_TYPE_CLICKABLECOMPONENT_HPP

#include "Components.hpp"

namespace ecs
{
    enum ClickableType {
        MULTIPLAYER,
        LAUNCH,
        SETTINGS,
        EXIT,
        INPUT,
        MOVE_UP,
        MOVE_DOWN,
        MOVE_RIGHT,
        MOVE_LEFT,
        AUTO_FIRE,
        BACK,
        DIFFICULTY,
        BONUS,
        NUMBER_OF_PLAYER,
        LOBBY,
        LEVEL
    };

    class ClickableComponent : public Component {
      public:
        ClickableComponent(ClickableType type);

        ClickableType getClickableType();
        void setClickableType(ClickableType type);

      private:
        ClickableType _type;
    };
} // namespace ecs

#endif // R_TYPE_CLICKABLECOMPONENT_HPP
