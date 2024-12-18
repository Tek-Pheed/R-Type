/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AssetManager
*/

#ifndef TINY_V8_ASSET_MANAGER
#define TINY_V8_ASSET_MANAGER

#include <any>
#include <memory>
#include "Engine.hpp"
#include <unordered_map>

// namespace Engine
// {
//     namespace Feature
//     {

//         class AssetManager : public AEngineFeature {
//           public:
//             explicit AssetManager(Core &engineRef);
//             ~AssetManager();

//             template<typename BackendType>
//             BackendType &loadAsset(const std::string &path, const std::string &identifier);

//             template<typename BackendType>
//             BackendType &getAsset(const std::string &path, const std::string &identifier);

//           protected:
//             void onStart(void) override;
//             void onTick(float deltaTimeSec) override;
//             void onStop(void) override;

//             std::unordered_map<std::type_index, std::unordered_map<std::string, std::any>> _assets;

//             //std::unordered_map<std::string, std::unique_ptr<BackendTypes>...> _content;
//         };

//     } // namespace Feature

// } // namespace Engine

#endif /* TINY_V8_ASSET_MANAGER */