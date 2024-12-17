/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game Engine
*/

#ifndef TINY_V8_HPP
#define TINY_V8_HPP

#include <cstdint>
#include <memory>
#include <mutex>
#include <typeindex>

#include <unordered_map>

namespace Engine
{
    static constexpr uint16_t DEFAULT_TICK_RATE = 60;
    class Core;

    class AEngineFeature {
      public:
        explicit AEngineFeature(Core &engineRef) : _engineRef(engineRef) {};
        ~AEngineFeature() {};

        virtual void onStart(void) = 0;
        virtual void onTick(void) = 0;
        virtual void onStop(void) = 0;

      protected:
        Core &_engineRef;
    };

    class Core {
        enum UpdateMode { UNLOCKED, LOCKED };

      public:
        Core();
        ~Core();

        void start(void);
        void stop(void);
        void setUpdateMode(UpdateMode mode);
        UpdateMode getUpdateMode(void);
        void setTickRate(uint16_t tickRateHZ);
        uint16_t getTickRate(void);

        template <class EngineFeature> EngineFeature &getFeature(void)
        {
            return (*(static_cast<EngineFeature *>(
                _features.at(typeid(EngineFeature)).get())));
        }

        template <class EngineFeature> EngineFeature &loadFeature(void)
        {
            _features.emplace(
                typeid(EngineFeature), std::make_unique<EngineFeature>(*this));
            return (getFeature<EngineFeature>());
        }

        template <class EngineFeature> bool isFeatureLoaded(void)
        {
            if (_features.find(typeid(EngineFeature)) != _features.end())
                return (true);
            return (false);
        }

        template <class EngineFeature> bool unloadFeature(void)
        {
            if (isFeatureLoaded<EngineFeature>()) {
                _features.erase(typeid(EngineFeature));
                return (true);
            }
            return (false);
        }

      protected:
        void mainLoop(void);

        bool _running = false;
        UpdateMode _updateMode = LOCKED;
        uint16_t _tickRate = DEFAULT_TICK_RATE;
        std::mutex _engineLock;

      private:
        std::unordered_map<std::type_index, std::unique_ptr<AEngineFeature>>
            _features;
    };

} // namespace Engine

#endif /* TINY_V8_HPP */