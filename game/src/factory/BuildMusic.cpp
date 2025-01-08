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
    ecs::Entity &Factory::buildMusic(sf::SoundBuffer &soundBuffer)
    {
        auto &music = _game.refEntityManager.getCurrentLevel().createEntity();

        sf::Sound a;
        music.addComponent(std::make_shared<ecs::MusicComponent<sf::Sound>>(a));

        auto &s = music.getComponent<ecs::MusicComponent<sf::Sound>>()
                     ->getMusicType();

        s.setBuffer(soundBuffer);
        s.setLoop(true);
        s.setVolume(25.0f);
        s.play();
        return (music);
    }
} // namespace RType
