/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MouseClicked.cpp
*/

#include <SFML/Graphics.hpp>
#include "Config.hpp"
#include "Events.hpp"
#include "components/ClickableComponent.hpp"

namespace RType
{
    void EventManager::mouseClicked()
    {
        Config config("config.cfg");
        sf::Vector2f mousePos = _game->getWindow().mapPixelToCoords(
            sf::Mouse::getPosition(_game->getWindow()));
        bool currentAutoFireValue = config.getAutoFireConfig();

        for (auto &entity :
            _game->refEntityManager.getCurrentLevel().getEntities()) {
            auto button = entity.get().getComponent<ecs::RenderComponent>();
            if (!button
                || (button->getObjectType()
                        != ecs::RenderComponent::ObjectType::BUTTON
                    && button->getObjectType()
                        != ecs::RenderComponent::ObjectType::INPUT))
                continue;

            auto rectangle =
                entity.get()
                    .getComponent<
                        ecs::RectangleComponent<sf::RectangleShape>>();
            if (!rectangle)
                continue;

            sf::FloatRect currentHover =
                rectangle->getRectangle().getGlobalBounds();
            if (currentHover.contains(mousePos)) {
                auto clickableType =
                    entity.get().getComponent<ecs::ClickableComponent>();

                if (!clickableType)
                    continue;

                switch (clickableType->getClickableType()) {
                    case ecs::ClickableType::MULTIPLAYER:
                        _game->_buttonList.clear();
                        _game->_inputList.clear();
                        _game->levelContinueMenu();
                        break;
                    case ecs::ClickableType::LAUNCH:
                        _game->_buttonList.clear();
                        _game->_inputList.clear();
                        _game->connectToGame();
                        break;
                    case ecs::ClickableType::EXIT:
                        _game->getWindow().close();
                        _game->refGameEngine.stop();
                        break;
                    case ecs::ClickableType::SETTINGS:
                        _game->_buttonList.clear();
                        _game->_inputList.clear();
                        _game->levelSettingsMenu();
                        break;
                    case ecs::ClickableType::AUTO_FIRE:
                        _game->handleAutoFireButton(
                            currentAutoFireValue ? "false" : "true", entity);
                        break;
                    case ecs::ClickableType::BACK:
                        _game->_buttonList.clear();
                        _game->_inputList.clear();
                        _game->levelMainMenu();
                        break;
                    case ecs::ClickableType::MOVE_UP:
                        _game->_isSettingsUpButtonClicked = true;
                        _game->_lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::MOVE_DOWN:
                        _game->_isSettingsDownButtonClicked = true;
                        _game->_lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::MOVE_LEFT:
                        _game->_isSettingsLeftButtonClicked = true;
                        _game->_lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::MOVE_RIGHT:
                        _game->_isSettingsRightButtonClicked = true;
                        _game->_lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::INPUT:
                        _game->_isSettingsNicknameButtonClicked = true;
                        _game->_lastInputIdClicked = entity.get().getID();
                        _game->_nicknameKeys.clear();
                        break;
                    default: break;
                }

                /*switch (str2int(text->getStr().c_str())) {
                    case str2int("PLAY"):
                        _game->levelContinueMenu();
                        break;
                    case str2int("EXIT"):
                        _game->getWindow().close();
                        _game->refGameEngine.stop();
                        break;
                    case str2int("SETTINGS"):
                        _game->levelSettingsMenu();
                        break;
                    case str2int("PLAY GAME"):
                        _game->connectToGame();
                        break;
                    case str2int("BACK"): _game->levelMainMenu(); break;
                    default:
                        if (text->getStr().find("Move up") == 0) {
                            _game->_isSettingsUpButtonClicked = true;
                            _game->_lastButtonIdClicked = entity.get().getID();
                        } else if (text->getStr().find("Move right") == 0) {
                            _game->_isSettingsRightButtonClicked = true;
                            _game->_lastButtonIdClicked = entity.get().getID();
                        } else if (text->getStr().find("Move left") == 0) {
                            _game->_isSettingsLeftButtonClicked = true;
                            _game->_lastButtonIdClicked = entity.get().getID();
                        } else if (text->getStr().find("Move down") == 0) {
                            _game->_isSettingsDownButtonClicked = true;
                            _game->_lastButtonIdClicked = entity.get().getID();
                        } else if (text->getStr().find("NICKNAME") == 0) {
                            _game->_isSettingsNicknameButtonClicked = true;
                            _game->_lastInputIdClicked = entity.get().getID();
                        } else if (text->getStr().find("Auto fire") == 0) {
                            bool currentAutoFireValue =
                                config.getAutoFireConfig();
                            std::string newAutoFireValue =
                                currentAutoFireValue ? "false" : "true";
                            _game->handleAutoFireButton(
                                newAutoFireValue, entity);
                        }
                        break;
                }*/
            }
        }
    }
} // namespace RType
