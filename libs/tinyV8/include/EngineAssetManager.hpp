/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AssetManager
*/

/**
 * @file Built-in asset manager of the game engine.
 */

#ifndef TINY_V8_ASSET_MANAGER
#define TINY_V8_ASSET_MANAGER

#include <any>
#include <exception>
#include <stdexcept>
#include <utility>
#include "Engine.hpp"
#include <unordered_map>

namespace Engine
{
    namespace Events
    {
        // Event asset loaded, ARG: std::string (asset name)
        constexpr auto EVENT_OnAssetLoaded{"onAssetLoaded"};

        // Event asset unloaded, ARG: std::string (asset name)
        constexpr auto EVENT_OnAssetUnloaded{"onAssetUnloaded"};
    }; // namespace Events

    namespace Feature
    {

        /**
         * @brief BuiltIn asset manager class (Engine Feature).
         */
        class AssetManager : public AEngineFeature {
          public:
            explicit AssetManager(Core &engineRef);
            AssetManager(AssetManager &&other) = default;
            ~AssetManager() override;

            /**
             * @brief Make the manager load an asset.
             *
             * @tparam BackendType: The type of your asset class.
             * @tparam LoadFncType: A memeber class function pointer to the
             loader function. This function will be invoked by the manager
             using a new constructed object.
             * @tparam Args: Extra argument types for passing to the loader
             function
             * @param path: A string containing the path of the asset.
             * @param identifier: An identifier for this asset. You will use
             this to retreive the asset later.
             * @param extraArgs: Extra argument that will be passed to the
             loader function.
             * @return BackendType&: A reference to the loaded and constructed
             object.

             @note: The manager is responsible for constructing the backend
             object.
             @note: The function will ignore the request if the asset
             was already loaded.
             */
            template <typename BackendType, typename LoadFncType,
                typename... Args>
            BackendType &loadAsset(const std::string &path,
                const std::string &identifier,
                LoadFncType(BackendType::*function), Args... extraArgs)
            {
                if (_assets.contains(typeid(BackendType))) {
                    if (_assets[typeid(BackendType)].contains(identifier)) {
                        BackendType *object = std::any_cast<BackendType>(
                            &(_assets[typeid(BackendType)].at(identifier)));
                        return (*object);
                    }
                }
                _assets[typeid(BackendType)].insert(
                    std::make_pair<std::string, BackendType>(
                        std::string(identifier), BackendType()));
                BackendType *object = std::any_cast<BackendType>(
                    &(_assets[typeid(BackendType)].at(identifier)));
                ((object)->*function)(path, extraArgs...);
                AEngineFeature::_engineRef.triggerEvent(
                    Events::EVENT_OnAssetLoaded, std::string(identifier));
                return (*object);
            }

            /**
             * @brief Returns an asset which was previously loaded, given its
             identifier.

             * @tparam BackendType: The type of your asset class.
             * @param identifier: The string that identifies this asset.
             * @return BackendType&: A reference to the asset object.
             */
            template <typename BackendType>
            BackendType &getAsset(const std::string &identifier)
            {
                if (_assets.contains(typeid(BackendType))) {
                    if (_assets[typeid(BackendType)].contains(identifier)) {
                        BackendType *object = std::any_cast<BackendType>(
                            &(_assets[typeid(BackendType)].at(identifier)));
                        return (*object);
                    }
                }
                throw std::out_of_range(THROW_ERROR_LOCATION
                    "AssetManager: Unable to find requested asset: "
                    + identifier
                    + ", make sure to load the asset before requesting it !");
            }

            /**
             * @brief Unloads an asset.

             * @tparam BackendType: The type of your asset class.
             * @param identifier: The string that identifies this asset.
             * @return true: The asset was sucessfully unloaded.
             * @return false: The asset could not be unloaded.
             */
            template <typename BackendType>
            bool unloadAsset(const std::string &identifier)
            {
                if (!_assets.contains(typeid(BackendType))) {
                    return (false);
                }
                _assets[typeid(BackendType)].erase(identifier);
                AEngineFeature::_engineRef.triggerEvent(
                    Events::EVENT_OnAssetUnloaded, std::string(identifier));
                return (true);
            }

          protected:
            void engineOnStart(void) override;
            void engineOnTick(float deltaTimeSec) override;
            void engineOnStop(void) override;
            void engineOnPostTick(float deltaTimeSec) override
            {
                (void) deltaTimeSec;
                return;
            };

            std::unordered_map<std::type_index,
                std::unordered_map<std::string, std::any>>
                _assets;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_ASSET_MANAGER */
