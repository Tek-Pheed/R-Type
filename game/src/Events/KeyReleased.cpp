/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** KeyReleased.cpp
*/

#include "Config.hpp"
#include "Events.hpp"
#include "Utils.hpp"

namespace RType
{
    void EventManager::keyReleased(sf::Event &event)
    {
        Config config("config.cfg");
        Utils utils;

        std::string moveUpKeyString = config.getKeyFromConfig("MOVE_UP");
        std::string moveRightKeyString = config.getKeyFromConfig("MOVE_RIGHT");
        std::string moveLeftKeyString = config.getKeyFromConfig("MOVE_LEFT");
        std::string moveDownKeyString = config.getKeyFromConfig("MOVE_DOWN");

        sf::Keyboard::Key moveUpKey = utils.getKeyFromString(moveUpKeyString);
        sf::Keyboard::Key moveRightKey =
            utils.getKeyFromString(moveRightKeyString);
        sf::Keyboard::Key moveLeftKey =
            utils.getKeyFromString(moveLeftKeyString);
        sf::Keyboard::Key moveDownKey =
            utils.getKeyFromString(moveDownKeyString);

        auto &player = _game.getLocalPlayer();
        auto velocity = player.getComponent<ecs::VelocityComponent>();
        if (event.key.code == moveUpKey || event.key.code == moveDownKey) {
            velocity->setVy(0.0f);
        }
        if (event.key.code == moveLeftKey || event.key.code == moveRightKey) {
            velocity->setVx(0.0f);
        }
    }
} // namespace RType
