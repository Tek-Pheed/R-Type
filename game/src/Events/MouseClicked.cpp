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

void handleNumberOfPlayerButton(ecs::Entity &entity)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!text)
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string maxPlayer = str.substr(separator + 1);
        int maxPlayerNB = std::atoi(maxPlayer.c_str());
        if (maxPlayerNB >= 10) {
            maxPlayerNB = 4;
        } else {
            maxPlayerNB++;
        }
        ss << maxPlayerNB;
        text->setStr("NUMBER OF PLAYER : " + ss.str());
    }
}

void handleDifficultyButton(ecs::Entity &entity)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!text)
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string difficulty = str.substr(separator + 2);
        if (difficulty == "EASY") {
            text->setStr("DIFFICULTY : MEDIUM");
        } else if (difficulty == "MEDIUM") {
            text->setStr("DIFFICULTY : HARD");
        } else {
            text->setStr("DIFFICULTY : EASY");
        }
    }
}

void handleBonusButton(ecs::Entity &entity)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!text)
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string bonus = str.substr(separator + 2);
        if (bonus == "ON") {
            text->setStr("BONUS : NO");
        } else {
            text->setStr("BONUS : YES");
        }
    }
}

namespace RType
{
    void EventManager::mouseClicked()
    {
        sf::Vector2f mousePos = _game.getWindow().mapPixelToCoords(
            sf::Mouse::getPosition(_game.getWindow()));
        bool currentAutoFireValue = _game._gameConfig.getAutoFireConfig();

        for (auto &entity :
            _game.refEntityManager.getCurrentLevel().getEntities()) {
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

                auto text =
                    entity.get().getComponent<ecs::TextComponent<sf::Text>>();

                if (!clickableType)
                    continue;

                switch (clickableType->getClickableType()) {
                    case ecs::ClickableType::MULTIPLAYER:
                        _game._buttonList.clear();
                        _game._inputList.clear();
                        _game.levelContinueMenu();
                        return;
                    case ecs::ClickableType::LAUNCH:
                        _game._buttonList.clear();
                        _game.connectToGame();
                        break;
                    case ecs::ClickableType::EXIT:
                        _game.getWindow().close();
                        _game.refGameEngine.stop();
                        break;
                    case ecs::ClickableType::SETTINGS:
                        _game._buttonList.clear();
                        _game._inputList.clear();
                        _game.levelSettingsMenu();
                        return;
                    case ecs::ClickableType::AUTO_FIRE:
                        _game.handleAutoFireButton(
                            currentAutoFireValue ? "false" : "true", entity);
                        break;
                    case ecs::ClickableType::BACK:
                        _game._buttonList.clear();
                        _game._inputList.clear();
                        _game.levelMainMenu();
                        return;
                    case ecs::ClickableType::MOVE_UP:
                        _game._isSettingsUpButtonClicked = true;
                        _game._lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::MOVE_DOWN:
                        _game._isSettingsDownButtonClicked = true;
                        _game._lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::MOVE_LEFT:
                        _game._isSettingsLeftButtonClicked = true;
                        _game._lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::MOVE_RIGHT:
                        _game._isSettingsRightButtonClicked = true;
                        _game._lastButtonIdClicked = entity.get().getID();
                        break;
                    case ecs::ClickableType::INPUT:
                        _game._isSettingsNicknameButtonClicked = true;
                        _game._lastInputIdClicked = entity.get().getID();
                        _game._nicknameKeys.clear();
                        break;
                    case ecs::ClickableType::NUMBER_OF_PLAYER:
                        handleNumberOfPlayerButton(entity);
                        break;
                    case ecs::ClickableType::DIFFICULTY:
                        handleDifficultyButton(entity);
                        break;
                    case ecs::ClickableType::BONUS:
                        handleBonusButton(entity);
                        break;
                    default: break;
                }
            }
        }
    }
} // namespace RType
