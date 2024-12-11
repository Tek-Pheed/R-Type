# Components

Components are one of the 3 main parts of ECS.
They enable [entities](./entity.md) to have dynamic data that we can retrieve
or define.

## Example of a component

PlayerComponent is used to define an entity as a player.

```cpp
#include "Components.hpp"

ecs::PlayerComponent::PlayerComponent(const std::string &name)
{
    this->_name = name;
}

std::string ecs::PlayerComponent::getName() const
{
    return this->_name;
}

void ecs::PlayerComponent::setName(const std::string &name)
{
    this->_name = name;
}

```

## Components list

| Component          | Description                   |
|--------------------|-------------------------------|
| Position component | Store position of an entity   |
| Player component   | Define entity as a player     |
| Velocity component | Store velocity of an entity   |
| Health component   | Store health of a entity      |
| Boss component     | Define entity as a boss       |
| Enemy component    | Define entity as an enemy     |
| Render component   | Renders an entity             |
| Text component     | Store text of an entity       |
| Bullet component   | Defines an entity as a bullet |
