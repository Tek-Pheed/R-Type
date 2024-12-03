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
        explicit PlayerComponent(const std::string &name);

        std::string getName() const;
        void setName(const std::string &name);

      private:
        std::string _name;
  	};

    class VelocityComponent : public Component {
      public:
        VelocityComponent(int vx, int vy);

        int getVx();
        int getVy();

        void setVx(int vx);
        void setVy(int vy);

      private:
        int _vx;
        int _vy;
	  };

    class HealthComponent : public Component {
      public:
        HealthComponent();
        explicit HealthComponent(int health);

        int getHealth();
        void setHealth(int health);

      private:
        int _health;
    };

    class BossComponent : public Component {
      public:
        explicit BossComponent(const std::string &name);

        std::string getName() const;
        void setName(const std::string &name);

      private:
        std::string _name;
    };

    enum ObjectType { CIRCLE, RECTANGLE, SPRITE, TEXT };

    class RenderComponent : public Component {
      public:
        RenderComponent(ObjectType type);

        ObjectType getType() const {
          return _type;
        }

        void setType(ObjectType type) {
          _type = type;
        }

      private:
        ObjectType _type;
    };
}

#endif // R_TYPE_COMPONENT_HPP
