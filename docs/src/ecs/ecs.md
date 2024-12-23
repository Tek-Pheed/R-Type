# Entity Component System (ECS)

An Entity-Component System (ECS) is a software design pattern used in game
development and other simulations.

It organizes data and behavior into entities (objects), components (data), and
systems (logic).

This approach improves flexibility and performance by separating data from
functionality.

All classes of this libs are in the namespace ``ecs``.

## Documentation of each parts

- [Entity](./entity.md)
- [Components](./components.md)
- [Systems](./system.md)

## Exemple

In this exemple we create an entity with ``id = 0``, here the entity is a player
named ``Samy
``, it's position is in ``x = 100, y = 100`` and it's default velocity is ``0``.

```cpp
#include "Entity.hpp"
#include "Components.hpp"

int main(void)
{
    auto &player = std::make_shared<ecs::Entity>(0);
    
    player->addComponent(std::make_shared<ecs::PlayerComponent>());
    player->addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    return 0;
}
```