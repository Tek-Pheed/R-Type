/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Events
*/

#ifndef TINY_V8_EVENT_HPP
#define TINY_V8_EVENT_HPP

#include <functional>
#include "Engine.hpp"

// Work in progress TODO

namespace Engine
{

    class IEvent {
        /* Some comodity event types */
        enum EngineEventType {
            APP_START,
            BEGIN_PLAY,
            TICK,
            END_PLAY,
            APP_END
        };

        using EventAppStart = std::function<void(Core &engineCore)>;
        using EventBeginPlay = std::function<void(Core &engineCore)>;
        using EventTick =
            std::function<void(Core &engineCore, float deltaTimeSeconds)>;
        using EventEndPlay = std::function<void(Core &engineCore)>;
        using EventEnd = std::function<void(Core &engineCore)>;
        /**  */

      public:
      protected:
    };

    // class EventManager : AEngineFeature {
    //   public:
    //     explicit EventManager(Core &engineRef) : AEngineFeature(engineRef) {};
    //     ~EventManager();


    //     // template <typename TargetClass, typename... FncArgs>
    //     // void bindEvent(
    //     //     TargetClass *classParamRef, std::function<void(FncArgs...)>);


    //   protected:
    //     void onStart(void) override;
    //     void onTick(void) override;
    //     void onStop(void) override;
    // };

} // namespace Engine

#endif /* TINY_V8_EVENT_HPP */