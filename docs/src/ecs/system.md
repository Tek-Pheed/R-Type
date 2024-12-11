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
    std::vector<std::shared_ptr<ecs::Entity>> &entities,
    sf::RenderWindow *window, float deltaTime)
{
    (void) deltaTime;

    for (auto &entity : entities) {
        auto renderComponent = entity->getComponent<RenderComponent>();
        auto position = entity->getComponent<PositionComponent>();
        auto player = entity->getComponent<PlayerComponent>();

        if (player && position && renderComponent) {
            renderComponent->getSprite()->setPosition(
                position->getX(), position->getY());
            window->draw(*(renderComponent->getSprite()));
        }
    }
}
```