/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AssetManager
*/

#ifndef TINY_V8_ASSET_MANAGER
#define TINY_V8_ASSET_MANAGER

#include <any>
#include <utility>
#include "Engine.hpp"
#include <unordered_map>

namespace Engine
{
    namespace Feature
    {

        class AssetManager : public AEngineFeature {
          public:
            explicit AssetManager(Core &engineRef);
            ~AssetManager();

            template <typename BackendType>
            BackendType &getAsset(const std::string &identifier)
            {
                BackendType *object = std::any_cast<BackendType>(
                    &(_assets[typeid(BackendType)].at(identifier)));
                return (*object);
            }

            template <typename BackendType, typename LoadFncType,
                typename... Args>
            BackendType &loadAsset(const std::string &path,
                const std::string &identifier,
                LoadFncType(BackendType::*function), Args... extraArgs)
            {
                _assets[typeid(BackendType)].insert(
                    std::make_pair<std::string, BackendType>(
                        std::string(identifier), BackendType()));
                BackendType *object = std::any_cast<BackendType>(
                    &(_assets[typeid(BackendType)].at(identifier)));
                ((object)->*function)(path, extraArgs...);
                return (*object);
            }

            template <typename BackendType>
            bool unloadAsset(const std::string &identifier)
            {
                if (!_assets.contains(typeid(BackendType))) {
                    return (false);
                }
                _assets[typeid(BackendType)].erase(identifier);
                return (true);
            }

          protected:
            void onStart(void) override;
            void onTick(float deltaTimeSec) override;
            void onStop(void) override;

            std::unordered_map<std::type_index,
                std::unordered_map<std::string, std::any>>
                _assets;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_ASSET_MANAGER */