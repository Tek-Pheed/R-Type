/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Component.hpp
*/
#ifndef R_TYPE_COMPONENT_HPP
#define R_TYPE_COMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace ecs
{
    class Component {
      public:
        virtual ~Component() = default;
    };

    class PositionComponent : public Component {
      public:
        PositionComponent(float x = 0, float y = 0);

        float getX();
        float getY();
        void setX(float x);
        void setY(float y);

      private:
        float _x;
        float _y;
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

    class EnemyComponent : public Component {
      public:
        explicit EnemyComponent(const std::string &name);

        std::string getName() const;
        void setName(const std::string &name);

      private:
        std::string _name;
    };

    enum ObjectType { CIRCLE, RECTANGLE, SPRITE, TEXT };

    class RenderComponent : public Component {
      public:
        RenderComponent(ObjectType type, const sf::Texture &texture,
            sf::Vector2f position = sf::Vector2f(0, 0));

        ecs::ObjectType getObjectType() const;
        void setObjectType(ecs::ObjectType type);

        sf::Sprite *getSprite();

        sf::CircleShape *getCircleShape();

        sf::RectangleShape *getRectangleShape();

      private:
        ecs::ObjectType _type;
        sf::Sprite *sprite = nullptr;
        sf::CircleShape *circleShape = nullptr;
        sf::RectangleShape *rectangleShape = nullptr;
    };

    class TextComponent : public Component {
      public:
        TextComponent();
        void setText(const std::string &text);
        std::string getText() const;

      private:
        std::string _text;
    };

    class BulletComponent : public Component {
      public:
        BulletComponent();
        explicit BulletComponent(bool isFromPlayer);

        bool getIsFromPlayer();
        void setIsFromPlayer(bool isFromPlayer);

      private:
        bool _isFromPlayer;
    };
} // namespace ecs

#endif // R_TYPE_COMPONENT_HPP
