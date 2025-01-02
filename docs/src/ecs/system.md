# Systems

Systems are used to update the graphical interface through the components
present in an entity.

## Systems list

| Systems         | Description                 |
|-----------------|-----------------------------|
| Position system | Update position of a entity |
| Render system   | Render entity with sfml     |
| Bullet system   | Update bullet in game       |

## Exemple

In this exemple we can see a basic render system with ``sf::Sprite`` of the SFML
lib.

```cpp
#include <SFML/Graphics.hpp>
#include "Systems.hpp"
#include "Entity.hpp"
#include "Components.hpp"

void ecs::RenderSystem::update(
    std::vector<ecs::Entity &> &entities,
    sf::RenderWindow *window, float deltaTime)
{
    (void) deltaTime;

    for (auto &entity : entities) {
        auto position = entity->getComponent<ecs::PositionComponent>();
        auto player = entity->getComponent<ecs::PlayerComponent>();
        auto sprite = entity->getComponent<ecs::SpriteComponent<sf::Sprite>>();

        if (player && position && sprite) {
            sprite->getSprite().setPosition(
                position->getX(), position->getY());
            window->draw(sprite->getSprite());
        }
    }
}
```