/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

#ifndef TINY_V8_NETWORKING
#define TINY_V8_NETWORKING

#include <cstddef>
#include <functional>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <vector>
#include "Engine.hpp"
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"
#include <condition_variable>
#include <unordered_map>

namespace Engine
{
    namespace Feature
    {

        class Networking : public AEngineFeature {
          public:
            explicit Networking(Core &engineRef)
                : AEngineFeature(engineRef) {};
            ~Networking();

            // template <typename TargetClass, typename... FncArgs>
            // void bindEvent(
            //     TargetClass *classParamRef,
            //     std::function<void(FncArgs...)>);

          protected:
            void onStart(void) override;
            void onTick(float deltaTimeSec) override;
            void onStop(void) override;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_NETWORKING */