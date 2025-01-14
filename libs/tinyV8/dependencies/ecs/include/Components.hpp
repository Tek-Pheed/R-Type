/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Component.hpp
*/
#ifndef R_TYPE_COMPONENT_HPP
#define R_TYPE_COMPONENT_HPP

#include <SFML/Audio.hpp>
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

        int getOldHealth();
        void setOldHealth(int oldHealth);

      private:
        int _health;
        int _oldHealth;
    };

    class BossComponent : public Component {
      public:
        explicit BossComponent(size_t bossID);

        size_t getBossID() const;
        void setBossID(size_t bossID);

      private:
        size_t _bossID;
    };

    class EnemyComponent : public Component {
      public:
        explicit EnemyComponent(size_t enemyID, size_t type = 0);

        size_t getEnemyID() const;
        void setEnemyID(size_t enemyID);

        size_t getType() const;
        void setType(size_t type);

        int getWave() const;
        void setWave(int wave);

      private:
        size_t _enemyID;
        // 0 for enemy, 1 for shooter, 2 for boss
        size_t _type;
        int _wave;
    };

    class RenderComponent : public Component {
      public:
        enum ObjectType {
            CIRCLE,
            RECTANGLE,
            SPRITE,
            TEXT,
            SPRITEANDTEXT,
            BUTTON,
            INPUT
        };
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

    template <typename musicType> class MusicComponent : public Component {
      public:
        explicit MusicComponent(musicType &music, std::string str)
            : _music(music), _str(str)
        {
        }

        musicType &getMusicType()
        {
            return this->_music;
        }

        void setMusicType(musicType &music)
        {
            _music = music;
        }

      private:
        musicType _music;
        std::string _str;
    };

    template <typename spriteType> class SpriteComponent : public Component {
      public:
        SpriteComponent(spriteType &sprite, int offsetX = 0, int offsetY = 0,
            int maxX = 0, float delay = 0.0f, int startX = 0,
            bool once = false)
            : _sprite(sprite), _sizeX(offsetX), _sizeY(offsetY), _maxX(maxX),
              _delay(delay), _startX(startX), _once(once)
        {
            _elapsedTime = 0.0f;
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
        void setMaxX(int maxX)
        {
            this->_maxX = maxX;
        }
        void setElapsedTime(float elapsedTime)
        {
            this->_elapsedTime = elapsedTime;
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
        int getMaxX() const
        {
            return this->_maxX;
        }
        float getDelay() const
        {
            return this->_delay;
        }
        int getStartX() const
        {
            return this->_startX;
        }
        float getElapsedTime() const
        {
            return this->_elapsedTime;
        }
        bool getOnce() const
        {
            return this->_once;
        }

      private:
        spriteType _sprite;
        int _sizeX;
        int _sizeY;
        int _maxX;
        float _delay;
        int _startX;
        float _elapsedTime;
        bool _once;
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

        textType _text;

      private:
        std::string _str;
    };

    class BulletComponent : public Component {
      public:
        explicit BulletComponent(bool isFromPlayer, size_t type = 0);

        bool getIsFromPlayer();
        void setIsFromPlayer(bool isFromPlayer);

        size_t getType() const;
        void setType(size_t type);

      private:
        bool _isFromPlayer;
        // 0 for regular, 1 for boss
        size_t _type;
    };

    class NameComponent : public Component {
      public:
        explicit NameComponent(const std::string &name);

        std::string getName() const;
        void setName(const std::string &name);

      private:
        std::string _name;
    };

    enum Bonus { RAPIDFIRE = 0, SPEED = 1, HEAL = 2, NONE = -1 };

    class BonusComponent : public Component {
      public:
        explicit BonusComponent(const Bonus &bonus);
        BonusComponent(size_t bonusID, const Bonus &bonus);

        Bonus getBonus();
        void setBonus(const Bonus &bonus);

        void setBonusID(size_t bonusID);
        size_t getBonusID();

      private:
        size_t _bonusID;
        Bonus _bonus;
    };

    class HitboxComponent : public Component {
      public:
        explicit HitboxComponent(float width, float height);

        float getHeight() const;
        float getWidth() const;

        void setHeight(float height);
        void setWidth(float width);

      private:
        float _height;
        float _width;
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
