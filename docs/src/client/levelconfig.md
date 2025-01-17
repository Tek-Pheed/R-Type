# Level config

The levelConfig class is a parser for loading level files that will be used by the game.

The levelConfig class in only a parser and does not create game entities directly.

You can use this class to get an object-oriented description of the level file.

It's core function parseLevelConfig() produces a list containing the level COMMAND and it's arguments.

For example BONUS(0.4, 0.5, 1) will become: ["BONUS", ["0.4", "0.5", "1"]].