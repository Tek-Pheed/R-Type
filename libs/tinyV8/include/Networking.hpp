/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

#ifndef TINY_V8_NETWORKING
#define TINY_V8_NETWORKING

#include "Engine.hpp"

namespace Engine
{
    namespace Feature
    {

        class Networking : public AEngineFeature {
          public:
            explicit Networking(Core &engineRef)
                : AEngineFeature(engineRef) {};
            ~Networking();

          protected:
            void onStart(void) override;
            void onTick(float deltaTimeSec) override;
            void onStop(void) override;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_NETWORKING */