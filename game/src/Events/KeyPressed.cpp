/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** KeyPressed.cpp
*/

#include "Events.hpp"
#include "Config.hpp"
#include "Utils.hpp"

namespace RType {
    void EventManager::keyPressed(sf::Event &event) {
        Config config("config.cfg");
        Utils utils;

        std::string moveUpKeyString = config.getKeyFromConfig("MOVE_UP");
        std::string moveRightKeyString = config.getKeyFromConfig("MOVE_RIGHT");
        std::string moveLeftKeyString = config.getKeyFromConfig("MOVE_LEFT");
        std::string moveDownKeyString = config.getKeyFromConfig("MOVE_DOWN");

        sf::Keyboard::Key moveUpKey = utils.getKeyFromString(moveUpKeyString);
        sf::Keyboard::Key moveRightKey = utils.getKeyFromString(moveRightKeyString);
        sf::Keyboard::Key moveLeftKey = utils.getKeyFromString(moveLeftKeyString);
        sf::Keyboard::Key moveDownKey = utils.getKeyFromString(moveDownKeyString);

        if (_game->hasLocalPlayer()) {
            auto &player = _game->getLocalPlayer();
            auto velocity = player.getComponent<ecs::VelocityComponent>();
            if (event.key.code == moveUpKey) {
                velocity->setVy(-200.0f);
            } else if (event.key.code == moveDownKey) {
                velocity->setVy(200.0f);
            } else if (event.key.code == moveRightKey) {
                velocity->setVx(200.0f);
            } else if (event.key.code == moveLeftKey) {
                velocity->setVx(-200.0f);
            }
        }
    }
}