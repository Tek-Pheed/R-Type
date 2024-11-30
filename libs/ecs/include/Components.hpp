/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Component.hpp
*/
#ifndef R_TYPE_COMPONENT_HPP
#define R_TYPE_COMPONENT_HPP

#include <string>

namespace ecs
{
    class Component {
      public:
        virtual ~Component() = default;
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

	class PlayerComponent : public Component {
      public:
        PlayerComponent(const std::string &name);

        std::string getName() const;
        void setName(const std::string &name);

      private:
        std::string _name;
	};
} // namespace ecs

#endif // R_TYPE_COMPONENT_HPP
