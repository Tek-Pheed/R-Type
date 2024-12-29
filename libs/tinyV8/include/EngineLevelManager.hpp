/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** levelManager
*/

/**
 * @file Built-in level manager.
 */

#ifndef TINY_V8_LEVEL_MANAGER_HPP
#define TINY_V8_LEVEL_MANAGER_HPP

#include <stdexcept>
#include "Engine.hpp"
#include "EngineECSManager.hpp"
#include <unordered_map>

namespace Engine
{

    /**
     * @brief Namespace providing events for level operations.
     */
    namespace Events
    {
        // Event level starts, ARG: std::string (level name)
        constexpr auto EVENT_OnLevelStart{"onLevelStart"};

        // Event level ends, ARG: std::string (level name)
        constexpr auto EVENT_OnLevelEnd{"onLevelEnd"};

        // Event level created, ARG: std::string (level name)
        constexpr auto EVENT_OnLevelCreated{"onLevelCreated"};

        // Event level destroyed, ARG: std::string (level name)
        constexpr auto EVENT_OnLevelDestroyed{"onLevelDestroyed"};

        // Event switch level, ARG: std::string (new level)
        constexpr auto EVENT_OnLevelSwitch{"onLevelSwitch"};
    }; // namespace Events

    /**
     * @brief A level class.

     * This level class is inherited from ECSManager. It allows a user to store
     level-specific entities, and provides control for starting/pausing a
     level.
     * @tparam GameClass: The class of your game. This game class will be
         * used by subsystems so they can access the context of your game when
         * they are updated.
     * @note: Users should primarly look for the LevelManager class, which
     implements a manager for this class to simplify it's use when building a
     multi-level game.
     */
    template <typename GameClass>
    class Level : public Feature::ECSManager<GameClass> {
      public:
        explicit Level(Core &engineRef, const std::string &levelName)
            : Feature::ECSManager<GameClass>(engineRef), _levelName(levelName)
        {
            Feature::ECSManager<GameClass>::_engineRef.triggerEvent(
                Events::EVENT_OnLevelCreated, std::string(_levelName));
        };
        Level(Level &&other) = default;
        ~Level()
        {
            Feature::ECSManager<GameClass>::_engineRef.triggerEvent(
                Events::EVENT_OnLevelDestroyed, std::string(_levelName));
        };

        /**
         * @brief Returns the name of the level.

         * @return const std::string&: Name of the level.
         */
        const std::string &getLevelName(void) const
        {
            return (_levelName);
        }

        /**
         * @brief Start updating the level.
         * @note: This is not necessary to call this function if you are using
         * a LevelManager.
         */
        void startLevel(void)
        {
            _updateLevel = true;
            Feature::ECSManager<GameClass>::_engineRef.triggerEvent(
                Events::EVENT_OnLevelStart, std::string(_levelName));
        }

        /**
         * @brief Stop updating the level.
         * @note: This is not necessary to call this function if you are using
         * a LevelManager.
         */
        void stopLevel(void)
        {
            _updateLevel = false;
            Feature::ECSManager<GameClass>::_engineRef.triggerEvent(
                Events::EVENT_OnLevelEnd, std::string(_levelName));
        }

        // Do not call this function manually unless you have a good reason
        // for.
        void engineOnTick(float deltaTimeSec) override
        {
            if (_updateLevel) {
                Feature::ECSManager<GameClass>::engineOnTick(deltaTimeSec);
            }
        }

      protected:
        const std::string _levelName;
        bool _updateLevel = false;
    };

    namespace Feature
    {
        /**
         * @brief The level manager class. It provides function to manage
         multiple levels in your game (eg: creating levels, switching levels,
         deleting levels...).

         * This class also provides a persistent level, wich should contain all
         entities that do not depends on a specific level.
         * @tparam GameClass: The class of your game. This game class will be
         * used by subsystems so they can access the context of your game when
         * they are updated.
         */
        template <typename GameClass>
        class LevelManager : public AEngineFeature {
          public:
            explicit LevelManager(Core &engineRef)
                : AEngineFeature(engineRef),
                  _persistentLevel(ECSManager<GameClass>(engineRef)) {};
            ~LevelManager() {};

            /**
             * @brief Returns the Persistent Level object.

             * This class should store all entities that do not depends on a
             specific level.
             * @return ECSManager<GameClass>&: A reference to the persistent
             level object.
             */
            ECSManager<GameClass> &getPersistentLevel() const
            {
                return (_persistentLevel);
            }

            /**
             * @brief Creates a new level.

             * @param levelName: The name of the level (should be unique).
             * @return Level<GameClass>&: A reference to the created level.
             * @note: If a level with the same name already exists, the
             function will throw a std::range_error exception.
             */
            Level<GameClass> &createNewLevel(const std::string &levelName)
            {
                if (_levels.contains(levelName)) {
                    throw std::range_error(
                        "A level with the same name already exists !");
                }
                _levels.emplace(levelName,
                    std::move(Level<GameClass>(_engineRef, levelName)));
                return (_levels.at(levelName));
            }

            /**
             * @brief Deletes a level.
             * All enitites contained in the level will also be delete.

             * @param levelName: The name of the level to delete.
             * @return true: The level was deleted successfully.
             * @return false: The level could not be deleted.
             */
            bool deleteLevel(const std::string &levelName)
            {
                const std::string cur = _currentLevel;

                if (!_levels.contains(levelName)) {
                    return (false);
                }
                _levels.erase(levelName);
                if (levelName == cur)
                    _currentLevel = "";
                return (true);
            }

            /**
             * @brief Get the current level name.
             *
             * @return const std::string&: A reference to a string containing
             * the name of the currently loaded level.
             * @note: The level name will be an empty string if no level was
             * switched into, or if it was previousely deleted.
             */
            const std::string &getCurrentLevelName(void) const
            {
                return (_currentLevel);
            }

            /**
             * @brief Returns the level specified by levelName. This function
             * does not perform any level-switching.

             * @param levelName: The name of the level to get.
             * @return Level<GameClass>&: A reference to the level.
                          * @note: This function will throw a std::range_error
             exception if
             * the level does not exists.
             */
            Level<GameClass> &getSpecificLevel(const std::string &levelName)
            {
                if (!_levels.contains(levelName)) {
                    throw std::range_error(
                        "No level was loaded, or the level does not exists !");
                }
                return (_levels.at(levelName));
            }

            /**
             * @brief Get the Current Level object.

             * @return Level<GameClass>&: A reference to the current level.
             * @note: This function will throw a std::range_error exception if
             * no level was switched into, or if it was previousely deleted.
             */
            Level<GameClass> &getCurrentLevel(void)
            {
                if (_currentLevel == "" || !_levels.contains(_currentLevel)) {
                    throw std::range_error(
                        "No level was loaded, or the level does not exists !");
                }
                return (_levels.at(_currentLevel));
            }

            /**
             * @brief Switch to a different level.

             * The switch will stop updating the previous level, and start
             updating the new level.
             * @param levelName: The name of the level to switch into.
             * @param destroyPrevious: Specify weather the previous level
             should be delete. (It will no longer be accessible, and all
             entities in the level will be destroyed).
             * @return Level<GameClass>&: The switched level, which is the new
             current level.
             * @note: This function will throw a std::range_error exception if
             the level with the specified name could not be found.
             */
            Level<GameClass> &switchLevel(
                const std::string &levelName, bool destroyPrevious = true)
            {
                const std::string prev = _currentLevel;

                if (!_levels.contains(levelName)) {
                    throw std::range_error(
                        "No level was loaded, or the level does not exists !");
                }
                AEngineFeature::_engineRef.triggerEvent(
                    Events::EVENT_OnLevelSwitch, std::string(levelName));
                _currentLevel = levelName;
                if (prev != "") {
                    _levels.at(prev).stopLevel();
                    if (destroyPrevious == true) {
                        deleteLevel(prev);
                    }
                }
                auto &l = getCurrentLevel();
                l.startLevel();
                return (l);
            }

          protected:
            ECSManager<GameClass> _persistentLevel;
            std::string _currentLevel;
            std::unordered_map<std::string, Level<GameClass>> _levels;

            void engineOnTick(float deltaTimeSec)
            {
                for (auto &[name, level] : _levels) {
                    level.engineOnTick(deltaTimeSec);
                }
            };
            void engineOnStop(void) {};
            void engineOnStart(void) {};
        };
    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_LEVEL_MANAGER_HPP */
