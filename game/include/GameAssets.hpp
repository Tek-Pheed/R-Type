/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameAssets
*/

#ifndef GAME_ASSETS_HPP
#define GAME_ASSETS_HPP

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <array>
#include <cstddef>
#include <typeindex>
#include <vector>
#include "Entity.hpp"
#include "Game.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace RType
{
    namespace Asset
    {
        struct AssetStore {
            const char *identifier;
            const char *path;
            std::type_index type;
        };

        constexpr auto PLAYER_TEXTURE = "playerTexture";
        constexpr auto ENEMY_TEXTURE = "enemyTexture";
        constexpr auto BACKGROUND_TEXTURE = "backgroundTexture";
        constexpr auto BULLET_TEXTURE = "bulletTexture";

        static const AssetStore assets[] = {
            {PLAYER_TEXTURE, "assets/sprites/r-typesheet42.gif",
                typeid(sf::Texture)},
            {ENEMY_TEXTURE, "assets/sprites/r-typesheet31.gif",
                typeid(sf::Texture)},
            {BACKGROUND_TEXTURE, "assets/background/background.png",
                typeid(sf::Texture)},
            {BULLET_TEXTURE, "./assets/sprites/r-typesheet1.gif",
                typeid(sf::Texture)}};

        template <typename Type>
        const std::vector<const AssetStore *> getAllAssetsOfType()
        {
            std::vector<const AssetStore *> vect;

            for (size_t i = 0; i < sizeof(assets) / sizeof(assets[0]); i++) {
                if (assets[i].type == typeid(Type)) {
                    vect.emplace_back(&assets[i]);
                }
            }
            return (vect);
        }

    }; // namespace Asset
}; // namespace RType

#endif /* GAME_ASSETS_HPP */