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
        float getOldY();
        float getOldX();
        void setX(float x);
        void setY(float y);
        void setOldX(float oldX);
        void setOldY(float oldY);

      private:
        float _x;
        float _y;
        float _oldX;
        float _oldY;
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
        VelocityComponent(float vx, float vy);

        float getVx();
        float getVy();

        void setVx(float vx);
        void setVy(float vy);

      private:
        float _vx;
        float _vy;
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

    template <typename spriteType> class SpriteComponent : public Component {
      public:
        SpriteComponent(spriteType sprite, int sizeX, int sizeY);

      private:
        spriteType _sprite;
        int _sizeX;
        int _sizeY;
    };

    template <typename circleType> class CircleComponent : public Component {
      public:
        CircleComponent(circleType circle, int radius);

        circleType getCircle() const;
        int getRadius() const;

        void setCircle(circleType circle);
        void setRadius(int radius);

      private:
        circleType _circle;
        int _radius;
    };

    template <typename rectangleType>
    class RectangleComponent : public Component {
      public:
        RectangleComponent(rectangleType rectangle, int sizeX, int sizeY);

        int getSizeX() const;
        int getSizeY() const;
        rectangleType getRectangle() const;

        void setSizeX(int sizeX);
        void setSizeY(int sizeY);
        void setRectangle(rectangleType rectangle);

      private:
        rectangleType _rectangle;
        int _sizeX;
        int _sizeY;
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
