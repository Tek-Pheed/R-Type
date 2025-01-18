# Factory

The client has a factory that can be used to build several types of entity, such
as a button, a player, an enemy, etc.

The factory doesn't impose a template, and each builder has its own number of
parameters.

## List of builder

| Builder          | Parameters                                                                          |
|------------------|-------------------------------------------------------------------------------------|
| Background       | none                                                                                | 
| Bonus            | id, x, y, bonusType                                                                 |
| Boss             | id, x, y, health, wave                                                              | 
| BulletFromPlayer | player id                                                                           |
| BulletFromEnemy  | enemy id                                                                            |
| BulletFromBoss   | boss id                                                                             |
| Button           | position, size, fillColor, outlineColor, text, characterSize, textColor, ButtonType |
| Enemy            | id, x, y, health, wave, velocityX, velociyY                                         |
| Explosion        | x, y                                                                                |
| Music            | soundBuffer, str                                                                    |
| Player           | isLocalPlayer, id, name                                                             |
| PlayerAI         | velocity, name, skinID                                                              |
| Text             | id, x, y, text, textColor, characterSize                                            |

