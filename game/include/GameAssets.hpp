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

#include <cstddef>
#include <typeindex>
#include <vector>
#include "Entity.hpp"
#include "Game.hpp"
#include "SFML/Audio.hpp"
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
        constexpr auto SHOOTERENEMY_TEXTURE = "shooterEnemyTexture";
        constexpr auto BOSS_TEXTURE = "bossTexture";
        constexpr auto BACKGROUND_TEXTURE = "backgroundTexture";
        constexpr auto BULLET_TEXTURE = "bulletTexture";
        constexpr auto BULLETENEMY_TEXTURE = "bulletEnemyTexture";
        constexpr auto BULLET_SOUND = "bulletSound";
        constexpr auto MENU_SONG = "menuSong";
        constexpr auto LOBBY_SONG = "lobbySong";
        constexpr auto GAME_SONG = "gameSong";
        constexpr auto BOSS_SONG = "bossSong";
        constexpr auto R_TYPE_FONT = "rTypeFont";
        constexpr auto BULLET_BOSS = "bossBullet";
        constexpr auto BULLET_BOSS_SOUND = "bossBulletSound";

        static const AssetStore assets[] = {
            {PLAYER_TEXTURE, "assets/sprites/r-typesheet42.gif",
                typeid(sf::Texture)},
            {ENEMY_TEXTURE, "assets/sprites/r-typesheet31.gif",
                typeid(sf::Texture)},
            {SHOOTERENEMY_TEXTURE, "assets/sprites/r-typesheet9.gif",
                typeid(sf::Texture)},
            {BOSS_TEXTURE, "assets/sprites/r-typesheet30.gif",
                typeid(sf::Texture)},
            {BACKGROUND_TEXTURE, "assets/background/nexus.png",
                typeid(sf::Texture)},
            {BULLET_TEXTURE, "assets/sprites/r-typesheet1.gif",
                typeid(sf::Texture)},
            {BULLETENEMY_TEXTURE, "assets/sprites/r-typesheet2.gif",
                typeid(sf::Texture)},
            {BULLET_SOUND, "assets/sounds/bullet.ogg",
                typeid(sf::SoundBuffer)},
            {MENU_SONG, "assets/sounds/menuMusic.ogg",
                typeid(sf::SoundBuffer)},
            {LOBBY_SONG, "assets/sounds/lobbyMusic.ogg",
                typeid(sf::SoundBuffer)},
            {GAME_SONG, "assets/sounds/streetFighter.ogg",
                typeid(sf::SoundBuffer)},
            {BOSS_SONG, "assets/sounds/bossMusic.ogg",
                typeid(sf::SoundBuffer)},
            {R_TYPE_FONT, "assets/font/r-type.ttf", typeid(sf::Font)},
            {BULLET_BOSS, "assets/sprites/r-typesheet30a.gif",
                typeid(sf::Texture)},
            {
                BULLET_BOSS_SOUND,
                "assets/sounds/bossBullet.ogg",
                typeid(sf::SoundBuffer),
            },
        };

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
