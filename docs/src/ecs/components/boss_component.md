
# Boss Component

This component contains the boss's name.

## Functions

- `BossComponent(const std::string &name)`: Construct the component with a given boss name.
- `getName() const`: Returns the boss's name.
- `setName(const std::string &name)`: Updates the boss's name.

## Variables

- `std::string _name`: Boss's name.

---

```cpp
class BossComponent : public Component {
  public:
    explicit BossComponent(const std::string &name);

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

ecs::BossComponent::BossComponent(const std::string &name)
{
    this->_name = name;
}

std::string ecs::BossComponent::getName() const
{
    return this->_name;
}

void ecs::BossComponent::setName(const std::string &name)
{
    this->_name = name;
}
```
