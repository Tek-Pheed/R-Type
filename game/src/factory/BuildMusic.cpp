/*
** EPITECH PROJECT, 2025
** R-Type [WSL: Ubuntu]
** File description:
** BuildMusic
*/

#include <sstream>
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "SFML/Audio/Sound.hpp"

namespace RType
{
    ecs::Entity &Factory::buildMusic(
        sf::SoundBuffer &soundBuffer, std::string str)
    {
        auto &music =
            _game.refEntityManager.getPersistentLevel().createEntity();

        sf::Sound a;
        music.addComponent(
            std::make_shared<ecs::MusicComponent<sf::Sound>>(a, str));

        auto &s = music.getComponent<ecs::MusicComponent<sf::Sound>>()
                      ->getMusicType();

        s.setBuffer(soundBuffer);
        s.setLoop(true);
        s.setVolume(GameInstance::MUSIC_VOLUME);
        return (music);
    }

    ecs::Entity &Factory::buildSoundEffect(
        sf::SoundBuffer &soundBuffer, std::string str, float volume)
    {
        auto &sound =
            _game.refEntityManager.getPersistentLevel().createEntity();

        sf::Sound a;
        sound.addComponent(
            std::make_shared<ecs::MusicComponent<sf::Sound>>(a, str));

        auto &s = sound.getComponent<ecs::MusicComponent<sf::Sound>>()
                      ->getMusicType();

        s.setBuffer(soundBuffer);
        s.setVolume(volume);
        s.play();
        return (sound);
    }
} // namespace RType
