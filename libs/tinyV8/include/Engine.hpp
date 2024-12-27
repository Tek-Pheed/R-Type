/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game Engine
*/

/**
 * @file The Engine Core file

 * This file contains the core class of the tinyV8 game engine.
 */

#ifndef TINY_V8_HPP
#define TINY_V8_HPP

#include <cstdint>
#include <memory>
#include <mutex>
#include <typeindex>

#include <unordered_map>

/**
 * @brief Engine Namespace

 * This namespace is dedicated to all Engine features, functions and constant
 variables.
 */
namespace Engine
{
    static constexpr uint16_t DEFAULT_TICK_RATE = 60;
    class Core;

    /**
     * @brief Engine feature base class.
     * This is the base class of all Engine features.
     * It provides three methods and a common interface for all engine
     features.

     * All engine features which needs to be loaded into the game engine Core
     class should use this abstract as a base class.
     * All derived classes are exepected to be constructed only via the
     loadFeature fucntion in the game engine Core class.
     * Do not construct these class manually, as events would not be fired
     correctly.
     */
    class AEngineFeature {
      public:
        explicit AEngineFeature(Core &engineRef) : _engineRef(engineRef) {};
        ~AEngineFeature() {};

        /**
         * @brief The start event.

         * This function will be called once the class was loaded into the game
         engine Core class.
         */
        virtual void onStart(void) = 0;

        /**
         * @brief The tick event.

         * This function will be called by the game engine Core class in the
         mainloop during a tick.
         * @param deltaTimeSec: Delta time between two ticks (in seconds).
         */
        virtual void onTick(float deltaTimeSec) = 0;

        /**
         * @brief The stop event.

         * This function will be called by the game engine Core class when the
         feature is from the engine unloaded.
         */
        virtual void onStop(void) = 0;

      protected:
        // Reference to the engine Core class.
        Core &_engineRef;
    };

    /**
     * @brief Game Engine Class
     * This is the core class of the TinyV8 game engine.

     * The engine is able to load and unload Features during runtime.
     * This class is essentially a manager for all engine features, it is
     * scalable thanks to the usage of templates for the core features.

     * External modules can be loaded and managed by tinyV8 into from your game
     if they support the simple ABI.
     */
    class Core {
        /**
         * @brief Tick update mode.
         * UNLOCKED: the tickrate will be unlimited.
         * LOCKED: the tickrate will be limited. You can use the setTickRate
         * function to change it.
         */
        enum UpdateMode { UNLOCKED, LOCKED };

      public:
        Core();
        ~Core();

        /**
         * @brief Sets the running value to true

         * This function allows the mainloop to keep looping
         */
        void start(void);

        /**
         * @brief Sets the running value to false

         * This functions allows the mainloop to stop
         */
        void stop(void);

        /**
         * @brief Set the Update Mode object
         * @param mode: LOCKED/UNLOCKED
         */
        void setUpdateMode(UpdateMode mode);

        /**
         * @brief Get the Update Mode object
         * @return UpdateMode: LOCKED/UNLOCKED
         */
        UpdateMode getUpdateMode(void) const;

        /**
         * @brief Set the Tick Rate variable

         * Sets the tick rate in hertz
         * @param tickRateHZ: Ticks per seconds
         */
        void setTickRate(uint16_t tickRateHZ);

        /**
         * @brief Get the Tick Rate
         * @return uint16_t: Ticks per seconds
         */
        uint16_t getTickRate(void) const;

        /**
         * @brief Loads a feature into the game engine.

         * This functions loads a specific game engine feature into this class,
         this is required since this class will take the ownership of the
         object. The feature will be updated inside the game engine mainloop
         using the defined tickrate.
         * @tparam EngineFeature: The engine feature class type.
         * @return EngineFeature&: A reference to the loaded engine feature
         object.
         */
        template <class EngineFeature> EngineFeature &loadFeature(void)
        {
            _features.emplace(
                typeid(EngineFeature), std::make_unique<EngineFeature>(*this));
            return (getFeature<EngineFeature>());
        }

        /**
         * @brief Gets the EngineFeature object

         * This functions returns the EngineFeature which was previously loaded
         into the engine.

         * This is useful if you don't want to keep a reference
         to the feature directly.
         * @tparam EngineFeature: The engine feature class type.
         * @return EngineFeature&: A reference to the engine feature object.

         * @note: If the feature was not previousely loaded into the engine
         using the loadFeature function, the function will likely throw.
         * You can use isFeatureLoaded to check if the feature was previousely
         loaded.
         */
        template <class EngineFeature> EngineFeature &getFeature(void)
        {
            return (*(static_cast<EngineFeature *>(
                _features.at(typeid(EngineFeature)).get())));
        }

        /**
         * @brief Checks wheather a feature was loaded into the game engine.
         *
         * @tparam EngineFeature: The engine feature class type.
         * @return true
         * @return false
         */
        template <class EngineFeature> bool isFeatureLoaded(void)
        {
            if (_features.find(typeid(EngineFeature)) != _features.end())
                return (true);
            return (false);
        }

        /**
         * @brief Unloads an engine feature.
         *
         * @tparam EngineFeature: The engine feature class type.
         * @return true: The feature was successfully unloaded.
         * @return false: The feature could not be unloaded.
         */
        template <class EngineFeature> bool unloadFeature(void)
        {
            if (isFeatureLoaded<EngineFeature>()) {
                _features.erase(typeid(EngineFeature));
                return (true);
            }
            return (false);
        }

        /**
         * @brief The game engine main loop.
         * This loop will update all game engine features and subsystems with a
         * constant tick rate.
         * @note: You can start and stop the main loop using the start and stop
         * functions.
         */
        void mainLoop(void);

      protected:
        /**
         * @brief Start event.

         * This event is called when the mainloop starts.
         */
        void onStart(void);

        /**
         * @brief Tick event.

         * This event is called every tick.
         * @param deltaTimeSec: Delta time between two ticks (in seconds).
         */
        void onTick(float deltaTimeSec);

        /**
         * @brief Stop event.

         * This event is called when the mainloop stops.
         */
        void onStop(void);

        // Controls whether the mainloop can run
        bool _running = true;

        // Engine Update mode
        UpdateMode _updateMode = LOCKED;

        // Engine Tick Rate, defaults to DEFAULT_TICK_RATE
        uint16_t _tickRate = DEFAULT_TICK_RATE;

        // Global engine lock (mutex)
        std::mutex _engineLock;

      private:
        // Map of engine features
        std::unordered_map<std::type_index, std::unique_ptr<AEngineFeature>>
            _features;
    };

} // namespace Engine

#endif /* TINY_V8_HPP */