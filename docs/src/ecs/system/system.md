# Systems

Systems for updating data contained in the various components

## Function

-

``void update(std::vector<std::shared_ptr<ecs::Entity>> &entities, sf::Window *window, float deltaTime)`` :
this function updates
an entity list by targeting the component with the window and deltaTime

## Systems list

| Systems         | Description                 |
|-----------------|-----------------------------|
| Position system | Update position of a entity |
| Render system   | Render entity with sfml     |
| Bullet system   | Update bullet in game       |