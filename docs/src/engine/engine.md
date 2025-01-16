# Engine

This is the base class of all Engine features.

It provides three methods and a common interface for all engine
features.

All engine features which needs to be loaded into the game engine Core
class should use this abstract as a base class.

All derived classes are exepected to be constructed only via the
loadFeature fucntion in the game engine Core class.

Do not construct these class manually, as events would not be fired
correctly.