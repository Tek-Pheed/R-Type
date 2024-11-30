/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Component.hpp
*/
#ifndef R_TYPE_COMPONENT_HPP
#define R_TYPE_COMPONENT_HPP

namespace ecs
{
    class Component {
      public:
        ~Component();
    };

    class PositionComponent : public Component {
      public:
        PositionComponent(int x, int y);

        int getX();
        int getY();
        void setX(int x);
        void setY(int y);

      private:
        int _x;
        int _y;
    };
} // namespace ecs

#endif // R_TYPE_COMPONENT_HPP
