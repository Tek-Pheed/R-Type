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

namespace RType
{
    ecs::Entity &Factory::buildMusic(sf::SoundBuffer &soundBuffer)
    {
        auto &music = _game.refEntityManager.getCurrentLevel().createEntity();
        sf::Sound sound;

        sound.setBuffer(soundBuffer);
        sound.setLoop(true);
        sound.setVolume(25.0f);
        sound.play();

        music.addComponent(std::make_shared<ecs::MusicComponent<sf::Sound>>(sound));

        return (music);
    }
} // namespace RType
