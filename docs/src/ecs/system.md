# Systems

The systems are used to update various game data such as collisions, positions,
display, life and projectiles.

Systems are built from an interface taking as parameters a list of entities and
the deltatime

## Systems list

| Systems           | Description                        |
|-------------------|------------------------------------|
| Position system   | Update position of a entity        |
| Render system     | Render entity with sfml            |
| Bullet system     | Update bullet in game              |
| Health system     | Update health in game              |
| Hitbox system     | Detect if item collide with hitbox |
| Background system | Managment of different background  |
