/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** KeyPressed.cpp
*/

#include "Config.hpp"
#include "Events.hpp"
#include "Factory.hpp"
#include "Utils.hpp"

namespace RType
{
    void EventManager::keyPressed(sf::Event &event)
    {
        Config config("config.cfg");
        Utils utils;
        Factory factory(_game);

        bool autoFireEnabled = config.getAutoFireConfig();
        sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;
        const size_t maxNicknameLength = 8;

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

        if (_game->_isSettingsUpButtonClicked) {
            keyPressed = event.key.code;
            _game->handleConfigButtons(keyPressed, 0);
            _game->_isSettingsUpButtonClicked = false;
        } else if (_game->_isSettingsRightButtonClicked) {
            keyPressed = event.key.code;
            _game->handleConfigButtons(keyPressed, -1);
            _game->_isSettingsRightButtonClicked = false;
        } else if (_game->_isSettingsLeftButtonClicked) {
            keyPressed = event.key.code;
            _game->handleConfigButtons(keyPressed, -2);
            _game->_isSettingsLeftButtonClicked = false;
        } else if (_game->_isSettingsDownButtonClicked) {
            keyPressed = event.key.code;
            _game->handleConfigButtons(keyPressed, -3);
            _game->_isSettingsDownButtonClicked = false;
        }

        } else if (_game->_isSettingsNicknameButtonClicked) {
            keyPressed = event.key.code;
            if (keyPressed == sf::Keyboard::Enter) {
                _game->handleNicknameButton(_game->_nicknameKeys);
                _game->_nicknameKeys.clear();
                _game->_isSettingsNicknameButtonClicked = false;
            }
            else if (keyPressed == sf::Keyboard::Backspace && !_game->_nicknameKeys.empty()) {
                _game->_nicknameKeys.pop_back();
                _game->handleNicknameButton(_game->_nicknameKeys);
            }
            else if (keyPressed >= sf::Keyboard::A && keyPressed <= sf::Keyboard::Z) {
                if (_game->_nicknameKeys.size() < maxNicknameLength) { 
                    _game->_nicknameKeys.push_back(keyPressed);
                    _game->handleNicknameButton(_game->_nicknameKeys);
                }
            }
        }
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

            if (!autoFireEnabled && event.key.code == sf::Keyboard::Space) {
                if (_game->getNetClientID() >= 0)
                    factory.buildBulletFromPlayer(
                        (size_t) _game->getNetClientID());
            }
        }
    }
} // namespace RType
