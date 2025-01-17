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

#define STRINGIZE(x)  STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define CATCH_ERROR_LOCATION \
    "\nCatch error at: " __FILE__ ":" STRINGIZE(__LINE__) ":\n\t"
#define THROW_ERROR_LOCATION \
    "\nThrow error at: " __FILE__ ":" STRINGIZE(__LINE__) ":\n\t"

#include <any>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <typeindex>
#include <vector>

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
     * @brief Namespace providing custom events
     */
    namespace Events
    {
        using EventType = std::string;
        using EventHanlderType =
            std::function<void(EventType, Core &, std::any)>;

        // Event engine before tick, ARG: float (delta time in seconds)
        constexpr auto EVENT_BeforeTick{"onBeforeTick"};

        // Event engine ticks, ARG: float (delta time in seconds)
        constexpr auto EVENT_OnTick{"onTick"};

        // Event mainloop starts, no arguments
        constexpr auto EVENT_OnStart{"onStart"};

        // Event mainloop stops, no arguments
        constexpr auto EVENT_OnStop{"onStop"};

        // Event engine after tick, ARG: float (delta time in seconds)
        constexpr auto EVENT_PostTick{"onPostTick"};

    }; // namespace Events

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
        virtual ~AEngineFeature() = default;
        explicit AEngineFeature(AEngineFeature &&other)
            : _engineRef(other._engineRef) {};

        /**
         * @brief The engine start event.

         * This function will be called once the class was loaded into the game
         engine Core class.
         */
        virtual void engineOnStart(void) = 0;

        /**
         * @brief The tick event.

         * This function will be called by the game engine Core class in the
         mainloop during a tick.
         * @param deltaTimeSec: Delta time between two ticks (in seconds).
         */
        virtual void engineOnTick(float deltaTimeSec) = 0;

        /**
        * @brief Built-In Post-Tick event.

        * This event is called every tick.
        * @param deltaTimeSec: Delta time between two ticks (in seconds).
        */
        virtual void engineOnPostTick(float deltaTimeSec) = 0;

        /**
         * @brief The stop event.

         * This function will be called by the game engine Core class when the
         feature is from the engine unloaded.
         */
        virtual void engineOnStop(void) = 0;

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

        bool verboseLogs = true;

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
         * @brief Return the last recorded delta time.

         * @return float: Delta time in seconds.
         */
        float getDeltaTime_Sec(void) const;

        /**
         * @brief The game engine main loop.
         * This loop will update all game engine features and subsystems with a
         * constant tick rate.
         * @note: You can start and stop the main loop using the start and stop
         * functions.
         */
        void mainLoop(void);

        /**
         * @brief Create a new event binding.

         * @param event: The event type.
         * @param eventHandler: The callback function.
         * @note: You can create multiple bindings for a single event type.
         */
        void addEventBinding(const Events::EventType &event,
            Events::EventHanlderType eventHandler);

        /**
         * @brief Create a new event binding.

         * @tparam T: The type of the class containing the event handler.
         * @param event: The event type.
         * @param eventHandler The callback function (class member function).
         * @param classRef: Instance of the class containing the callback.
         */
        template <class T>
        void addEventBinding(const Events::EventType &event,
            std::function<void(T *ptr, Events::EventType, Core &, std::any)>
                eventHandler,
            T &classRef)
        {
            auto fnc =
                std::bind(eventHandler, &classRef, std::placeholders::_1,
                    std::placeholders::_2, std::placeholders::_3);

            addEventBinding(event, fnc);
        }

        /**
         * @brief Removes all event binding of type.

         * @return true: The bindings were successfully removed.
         * @return false: Unable to remove bindings.
         */
        bool clearEventBinding(const Events::EventType &event);

        /**
         * @brief Triggers the given event.

         * @param event: The event to trigger.
         * @param extraArg: An extra argument to pass to the callback function.
         * @return true: The event was triggered successfully.
         * @return false: The event could not be triggered.
         */
        bool triggerEvent(
            const Events::EventType &event, std::any extraArg = {});

      protected:
        /**
         * @brief Built-In Start event.

         * This event is called when the mainloop starts.
         */
        void engineOnStart(void);

        /**
         * @brief Built-In Tick event.

         * This event is called every tick.
         * @param deltaTimeSec: Delta time between two ticks (in seconds).
         */
        void engineOnTick(float deltaTimeSec);

        /**
         * @brief Built-In Post-Tick event.

         * This event is called every tick.
         * @param deltaTimeSec: Delta time between two ticks (in seconds).
         */
        void engineOnPostTick(float deltaTimeSec);

        /**
         * @brief Built-In Stop event.

         * This event is called when the mainloop stops.
         */
        void engineOnStop(void);

        // Controls whether the mainloop can run
        bool _running = true;

        // Engine Update mode
        UpdateMode _updateMode = LOCKED;

        // Engine Tick Rate, defaults to DEFAULT_TICK_RATE
        uint16_t _tickRate = DEFAULT_TICK_RATE;

        // Global engine lock (mutex)
        std::mutex _engineLock;

        // Map of custom events
        std::unordered_map<Events::EventType,
            std::vector<Events::EventHanlderType>>
            _events;

      private:
        // Map of engine features
        std::unordered_map<std::type_index, std::unique_ptr<AEngineFeature>>
            _features;

        float _currentDeltaTime = 0.0f;
    };

} // namespace Engine

#endif /* TINY_V8_HPP */