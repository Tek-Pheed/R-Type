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
#include "Factory.hpp"
#include "Utils.hpp"

using namespace RType;

void EventManager::keyPressed(sf::Event &event)
{
    Utils utils;

    // bool autoFireEnabled = config.getAutoFireConfig();
    sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;
    const size_t maxInputLenght = 16;

    std::string moveUpKeyString =
        _game._gameConfig.getKeyFromConfig("MOVE_UP");
    std::string moveRightKeyString =
        _game._gameConfig.getKeyFromConfig("MOVE_RIGHT");
    std::string moveLeftKeyString =
        _game._gameConfig.getKeyFromConfig("MOVE_LEFT");
    std::string moveDownKeyString =
        _game._gameConfig.getKeyFromConfig("MOVE_DOWN");

    sf::Keyboard::Key moveUpKey = utils.getKeyFromString(moveUpKeyString);
    sf::Keyboard::Key moveRightKey =
        utils.getKeyFromString(moveRightKeyString);
    sf::Keyboard::Key moveLeftKey = utils.getKeyFromString(moveLeftKeyString);
    sf::Keyboard::Key moveDownKey = utils.getKeyFromString(moveDownKeyString);

    if (_game._isSettingsUpButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, 0);
        _game._isSettingsUpButtonClicked = false;
    } else if (_game._isSettingsRightButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, -1);
        _game._isSettingsRightButtonClicked = false;
    } else if (_game._isSettingsLeftButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, -2);
        _game._isSettingsLeftButtonClicked = false;
    } else if (_game._isSettingsDownButtonClicked) {
        keyPressed = event.key.code;
        _game.handleConfigButtons(keyPressed, -3);
        _game._isSettingsDownButtonClicked = false;
    } else if (_game._isSettingsNicknameButtonClicked) {
        keyPressed = event.key.code;
        if (keyPressed == sf::Keyboard::Enter) {
            _game.handleInputButtons(_game._nicknameKeys);
            _game._nicknameKeys.clear();
            _game._isSettingsNicknameButtonClicked = false;
        } else if (keyPressed == sf::Keyboard::Backspace
            && !_game._nicknameKeys.empty()) {
            _game._nicknameKeys.pop_back();
            _game.handleInputButtons(_game._nicknameKeys);
        } else if ((keyPressed >= sf::Keyboard::A
                       && keyPressed <= sf::Keyboard::Z)
            || (keyPressed >= sf::Keyboard::Num0
                && keyPressed <= sf::Keyboard::Num9)
            || (keyPressed >= sf::Keyboard::Numpad0
                && keyPressed <= sf::Keyboard::Numpad9)
            || keyPressed == sf::Keyboard::Period) {
            if (_game._nicknameKeys.size() < maxInputLenght) {
                _game._nicknameKeys.push_back(keyPressed);
                _game.handleInputButtons(_game._nicknameKeys);
            }
        }
    }
    if (event.key.code == sf::Keyboard::Escape) {
        _game.refGameEngine.triggerEvent(Engine::Events::EVENT_DisconnectedFromServer);
    }
    if (_game.hasLocalPlayer()) {
        auto &player = _game.getLocalPlayer();
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
