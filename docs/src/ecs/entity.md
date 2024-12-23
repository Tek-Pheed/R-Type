# Entity

Entities contain all the components that define this data.

## Functions

- ``void addComponent(std::shared_ptr<componentType> component)`` --> add a
  component to an entity
- ``std::shared_ptr<componentType> getComponent()`` --> get a
  specific component of an entity
- ``std::vector<std::type_index> getComponents()`` --> get all components of an
  entity
- ``void addComponent(std::shared_ptr<componentType> component)`` --> remove a
  component to an entity
- ``std::size_t getID() const`` --> getID of the entity
- ``void setID(std::size_t id) const`` --> set ID of an entity

## Exemple

In this exemple we can see how to create an entity, how to add component and how
to
get a component.

```cpp
#include <iostream>
#include "Entity.hpp"
#include "Components.hpp"

int main(void)
{
    auto &player = std::make_shared<ecs::Entity>(0);
    
    player->addComponent(std::make_shared<ecs::PlayerComponent>());
    player->addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));
    
    auto playerPosition = player->getComponent<ecs::PositionComponent>();
    
    std::cout << playerPosition->getX() << std::endl;
    return 0;
}
```