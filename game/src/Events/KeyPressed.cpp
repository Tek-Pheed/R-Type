/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** KeyPressed.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Config.hpp"
#include "Events.hpp"
#include "SFML/System/Vector2.hpp"
#include "Utils.hpp"

using namespace RType;

void EventManager::keyPressed(sf::Event &event)
{
    Utils utils;

    // bool autoFireEnabled = config.getAutoFireConfig();
    sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;
    const size_t maxInputLenght = 16;

    std::string moveUpKeyString = _game.gameConfig.getKeyFromConfig("MOVE_UP");
    std::string moveRightKeyString =
        _game.gameConfig.getKeyFromConfig("MOVE_RIGHT");
    std::string moveLeftKeyString =
        _game.gameConfig.getKeyFromConfig("MOVE_LEFT");
    std::string moveDownKeyString =
        _game.gameConfig.getKeyFromConfig("MOVE_DOWN");

    sf::Keyboard::Key moveUpKey = utils.getKeyFromString(moveUpKeyString);
    sf::Keyboard::Key moveRightKey =
        utils.getKeyFromString(moveRightKeyString);
    sf::Keyboard::Key moveLeftKey = utils.getKeyFromString(moveLeftKeyString);
    sf::Keyboard::Key moveDownKey = utils.getKeyFromString(moveDownKeyString);

    if (_game.isSettingsUpButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, 0);
        _game.isSettingsUpButtonClicked = false;
    } else if (_game.isSettingsRightButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, -1);
        _game.isSettingsRightButtonClicked = false;
    } else if (_game.isSettingsLeftButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, -2);
        _game.isSettingsLeftButtonClicked = false;
    } else if (_game.isSettingsDownButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, -3);
        _game.isSettingsDownButtonClicked = false;
    } else if (_game.isSettingsNicknameButtonClicked) {
        keyPressed = event.key.code;
        if (keyPressed == sf::Keyboard::Enter) {
            _game.handleInputButtons(_game.nicknameKeys);
            _game.nicknameKeys.clear();
            _game.isSettingsNicknameButtonClicked = false;
        } else if (keyPressed == sf::Keyboard::Backspace
            && !_game.nicknameKeys.empty()) {
            _game.nicknameKeys.pop_back();
            _game.handleInputButtons(_game.nicknameKeys);
        } else if ((keyPressed >= sf::Keyboard::A
                       && keyPressed <= sf::Keyboard::Z)
            || (keyPressed >= sf::Keyboard::Num0
                && keyPressed <= sf::Keyboard::Num9)
            || (keyPressed >= sf::Keyboard::Numpad0
                && keyPressed <= sf::Keyboard::Numpad9)
            || keyPressed == sf::Keyboard::Period) {
            if (_game.nicknameKeys.size() < maxInputLenght) {
                _game.nicknameKeys.push_back(keyPressed);
                _game.handleInputButtons(_game.nicknameKeys);
            }
        }
    }
    if (event.key.code == sf::Keyboard::Escape) {
        _game.refGameEngine.triggerEvent(
            Engine::Events::EVENT_DisconnectedFromServer);
    }
    if (_game.hasLocalPlayer()) {
        auto &player = _game.getLocalPlayer();
        auto velocity = player.getComponent<ecs::VelocityComponent>();
        auto bonus = player.getComponent<ecs::BonusComponent>();

        if (event.key.code == moveUpKey) {
            if (bonus->getBonus() == ecs::Bonus::SPEED) {
                velocity->setVy(-0.4f);
            } else {
                velocity->setVy(-0.30f);
            }
        } else if (event.key.code == moveDownKey) {
            if (bonus->getBonus() == ecs::Bonus::SPEED) {
                velocity->setVy(0.4f);
            } else {
                velocity->setVy(0.30f);
            }
        } else if (event.key.code == moveRightKey) {
            if (bonus->getBonus() == ecs::Bonus::SPEED) {
                velocity->setVx(0.24f);
            } else {
                velocity->setVx(0.16f);
            }
        } else if (event.key.code == moveLeftKey) {
            if (bonus->getBonus() == ecs::Bonus::SPEED) {
                velocity->setVx(-0.24f);
            } else {
                velocity->setVx(-0.16f);
            }
        }
    }
}
