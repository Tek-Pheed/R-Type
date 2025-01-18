# Level Manager

A level class.

This level class is inherited from ECSManager. It allows a user to store
level-specific entities, and provides control for starting/pausing a
level.

GameClass: The class of your game. This game class will be
used by subsystems so they can access the context of your game when
they are updated.

Users should primarly look for the LevelManager class, which
implements a manager for this class to simplify it's use when building a
multi-level game.
     
