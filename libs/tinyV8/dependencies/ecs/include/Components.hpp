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
        PlayerComponent(size_t playerID);

        size_t getPlayerID() const;
        void setPlayerID(size_t playerID);

      private:
        size_t _playerID;
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
        explicit BossComponent();
    };

    class EnemyComponent : public Component {
      public:
        explicit EnemyComponent(size_t enemyID);

        size_t getEnemyID() const;
        void setEnemyID(size_t enemyID);

      private:
        size_t _enemyID;
    };

    class RenderComponent : public Component {
      public:
        enum ObjectType { CIRCLE, RECTANGLE, SPRITE, TEXT };
        explicit RenderComponent(ObjectType type);

        ObjectType getObjectType() const;
        void setObjectType(ObjectType type);

      private:
        ObjectType _type;
    };

    class BackgroundComponent : public Component {
      public:
        explicit BackgroundComponent(float moveDelta = 0.1f);

        float getClock(void) const;
        float setClock(float delta);
        bool isTimeToMove(void);

      private:
        float _deltaCounter;
        const float _moveDelta;
    };

    template <typename spriteType> class SpriteComponent : public Component {
      public:
        SpriteComponent(spriteType &sprite, int sizeX, int sizeY)
            : _sprite(sprite), _sizeX(sizeX), _sizeY(sizeY)
        {
        }

        void setSprite(spriteType &sprite)
        {
            this->_sprite = sprite;
        }
        void setSizeX(int sizeX)
        {
            this->_sizeX = sizeX;
        }
        void setSizeY(int sizeY)
        {
            this->_sizeY = sizeY;
        }

        spriteType &getSprite()
        {
            return this->_sprite;
        }
        int getSizeX() const
        {
            return this->_sizeX;
        }
        int getSizeY() const
        {
            return this->_sizeY;
        }

      private:
        spriteType _sprite;
        int _sizeX;
        int _sizeY;
    };

    template <typename circleType> class CircleComponent : public Component {
      public:
        CircleComponent(circleType &circle, int radius)
            : _circle(circle), _radius(radius)
        {
        }

        circleType &getCircle()
        {
            return this->_circle;
        }
        int getRadius() const
        {
            return this->_radius;
        }

        void setCircle(circleType circle)
        {
            this->_circle = circle;
        }
        void setRadius(int radius)
        {
            this->_radius = radius;
        }

      private:
        circleType _circle;
        int _radius;
    };

    template <typename rectangleType>
    class RectangleComponent : public Component {
      public:
        RectangleComponent(rectangleType &rectangle, int sizeX, int sizeY)
            : _rectangle(rectangle), _sizeX(sizeX), _sizeY(sizeY)
        {
        }

        int getSizeX() const
        {
            return this->_sizeX;
        }
        int getSizeY() const
        {
            return this->_sizeY;
        }
        rectangleType &getRectangle()
        {
            return this->_rectangle;
        }

        void setSizeX(int sizeX)
        {
            this->_sizeX = sizeX;
        }
        void setSizeY(int sizeY)
        {
            this->_sizeY = sizeY;
        }
        void setRectangle(rectangleType rectangle)
        {
            this->_rectangle = rectangle;
        }

      private:
        rectangleType _rectangle;
        int _sizeX;
        int _sizeY;
    };

    template <typename textType> class TextComponent : public Component {
      public:
        TextComponent(textType &text, const std::string &str)
            : _text(text), _str(str)
        {
        }

        textType &getText()
        {
            return this->_text;
        }
        void setText(const textType &text)
        {
            this->_text = text;
        }

        std::string getStr() const
        {
            return this->_str;
        }
        void setStr(const std::string &str)
        {
            this->_str = str;
        }

      private:
        textType _text;
        std::string _str;
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

    class NameComponent : public Component {
      public:
        explicit NameComponent(const std::string &name);

        std::string getName() const;
        void setName(const std::string &name);

      private:
        std::string _name;
    };

	enum Bonus { RAPIDFIRE, DOUBLEFIRE, TRIPLEFIRE, ALLIES };

    class BonusComponent : public Component {
      public:
        explicit BonusComponent(const Bonus &bonus);

        Bonus getBonus();
        void setBonus(const Bonus &bonus);

      private:
        Bonus _bonus;
    };

    // class ReplicationComponent : public Component {
    //     enum ReplicationMode { NO_REPLICATION, OTHER_ONLY, MULTICAST };
    //     enum ReplicationMethod { FAST, RELIABLE };

    //   public:
    //     explicit ReplicationComponent(Entity &attachedEntity);

    //     template<typename PropertyType, typename FuncSign>
    //     void addPropertyReplication(std::function<FuncSign> getterFunction,
    //     )

    //   private:
    //     Entity &_attachedEntity;

    // };
} // namespace ecs

#endif // R_TYPE_COMPONENT_HPP
