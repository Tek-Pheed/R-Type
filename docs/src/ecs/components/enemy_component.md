
# Enemy Component

This component contains the enemy's name.

## Functions

- `EnemyComponent(const std::string &name)`: Construct the component with a given enemy name.
- `getName() const`: Returns the enemy's name.
- `setName(const std::string &name)`: Updates the enemy's name.

## Variables

- `std::string _name`: Enemy's name.

---

```cpp
class EnemyComponent : public Component {
  public:
    explicit EnemyComponent(const std::string &name);

    std::string getName() const;
    void setName(const std::string &name);

  private:
    std::string _name;
};
```

---

### Implementation

```cpp
#include "Components.hpp"

ecs::EnemyComponent::EnemyComponent(const std::string &name)
{
    this->_name = name;
}

std::string ecs::EnemyComponent::getName() const
{
    return this->_name;
}

void ecs::EnemyComponent::setName(const std::string &name)
{
    this->_name = name;
}
```
