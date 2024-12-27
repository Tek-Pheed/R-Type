/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

/**
 * @file: Built-in event manager of the game engine.
 */

#ifndef TINY_V8_EVENT
#define TINY_V8_EVENT

#include <functional>
#include "Engine.hpp"

namespace Engine
{
    namespace Feature
    {

        class EventManager : public AEngineFeature {
          public:
            explicit EventManager(Core &engineRef);
            ~EventManager();

            // template <typename TargetClass, typename... FncArgs>
            // void bindEvent(
            //     TargetClass *classParamRef,
            //     std::function<void(FncArgs...)>);

            template <class T>
            void bindOnStart(std::function<void(void)> onStartFunc, T &ref)
            {
                _onStartFunc = std::bind(onStartFunc, ref);
            }

            template <class T>
            void bindOnStop(std::function<void(void)> OnStopFunc, T &ref)
            {
                _onStopFunc = std::bind(OnStopFunc, ref);
            }

            template <class T>
            void bindOnTick(std::function<void(float)> OnTickFunc, T &ref)
            {
                _onTickFunc =
                    std::bind(OnTickFunc, ref, std::placeholders::_1);
            }

          protected:
            void onStart(void) override;
            void onTick(float deltaTimeSec) override;
            void onStop(void) override;

            std::function<void(void)> _onStartFunc;
            std::function<void(void)> _onStopFunc;
            std::function<void(float)> _onTickFunc;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_EVENT */