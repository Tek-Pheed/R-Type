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