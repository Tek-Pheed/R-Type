/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** buildSettingsMenu.cpp
*/

#include "Game.hpp"
#include "GameAssets.hpp"
#include "Levels.hpp"

using namespace RType;

void Levels::buildSettingsMenu()
{
    auto &level = _game.refEntityManager.createNewLevel("settingsMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(_game);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(_game);
    _game.refEntityManager.switchLevel("settingsMenu");

    if constexpr (!server) {
        auto &title = _game.refEntityManager.getCurrentLevel().createEntity();

        sf::Text text;
        text.setFont(
            _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.0f);
        text.setString("SETTINGS");

        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float) _game.getWindow().getSize().x;

        float posX = (windowWidth - textWidth) / 2;
        float posY = (float) _game.getWindow().getSize().y / 10;

        title.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT));
        title.addComponent(
            std::make_shared<ecs::PositionComponent>(posX, posY - 50));
        title.addComponent(
            std::make_shared<ecs::TextComponent<sf::Text>>(text, "SETTINGS"));

        std::string moveUpAction = _game.gameConfig.getConfig().at(1);
        std::string moveRightAction = _game.gameConfig.getConfig().at(2);
        std::string moveLeftAction = _game.gameConfig.getConfig().at(3);
        std::string moveDownAction = _game.gameConfig.getConfig().at(4);
        std::string autoFireAction = _game.gameConfig.getConfig().at(7);
        std::string resolutionAction = _game.gameConfig.getConfig().at(6);

        std::string endUpMove = "";
        std::string endRightMove = "";
        std::string endLeftMove = "";
        std::string endDownMove = "";
        std::string endAutoFire = "";
        std::string endResolution = "";

        size_t equalPosUp = moveUpAction.find("=");
        if (equalPosUp != std::string::npos) {
            std::string beforeUpEqual = "MOVE UP";
            std::string afterUpEqual = moveUpAction.substr(equalPosUp + 1);
            size_t firstdoublepoint = afterUpEqual.find("::");
            std::string afterfirstdoublepoint =
                afterUpEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endUpMove = beforeUpEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosRight = moveRightAction.find("=");
        if (equalPosRight != std::string::npos) {
            std::string beforeRightEqual = "MOVE RIGHT";
            std::string afterRightEqual =
                moveRightAction.substr(equalPosRight + 1);
            size_t firstdoublepoint = afterRightEqual.find("::");
            std::string afterfirstdoublepoint =
                afterRightEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endRightMove = beforeRightEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosLeft = moveLeftAction.find("=");
        if (equalPosLeft != std::string::npos) {
            std::string beforeLeftEqual = "MOVE LEFT";
            std::string afterLeftEqual =
                moveLeftAction.substr(equalPosLeft + 1);
            size_t firstdoublepoint = afterLeftEqual.find("::");
            std::string afterfirstdoublepoint =
                afterLeftEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endLeftMove = beforeLeftEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosDown = moveDownAction.find("=");
        if (equalPosDown != std::string::npos) {
            std::string beforeDownEqual = "MOVE DOWN";
            std::string afterDownEqual =
                moveDownAction.substr(equalPosDown + 1);
            size_t firstdoublepoint = afterDownEqual.find("::");
            std::string afterfirstdoublepoint =
                afterDownEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endDownMove = beforeDownEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosAutoFire = autoFireAction.find("=");
        if (equalPosAutoFire != std::string::npos) {
            std::string beforeAutoFireEqual = "AUTO FIRE";
            std::string afterAutoFireEqual =
                autoFireAction.substr(equalPosAutoFire + 1);
            endAutoFire = beforeAutoFireEqual + " : "
                + (afterAutoFireEqual == "true" ? "Yes" : "No");
        }

        size_t equalPosResolution = resolutionAction.find("=");
        if (equalPosResolution != std::string::npos) {
            std::string beforeResolutionEqual = "RESOLUTION";
            std::string afterResolutionEqual =
                resolutionAction.substr(equalPosResolution + 1);
            endResolution =
                beforeResolutionEqual + " : " + afterResolutionEqual;
        }

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 2),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endUpMove, 40, sf::Color::Black, ecs::ClickableType::MOVE_UP);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 1),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endDownMove, 40, sf::Color::Black, ecs::ClickableType::MOVE_DOWN);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 0),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endRightMove, 40, sf::Color::Black,
            ecs::ClickableType::MOVE_RIGHT);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -1),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endLeftMove, 40, sf::Color::Black, ecs::ClickableType::MOVE_LEFT);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -2),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endAutoFire, 40, sf::Color::Black, ecs::ClickableType::AUTO_FIRE);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -3),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endResolution, 40, sf::Color::Black,
            ecs::ClickableType::RESOLUTION);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -4),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "BACK",
            40, sf::Color::Black, ecs::ClickableType::BACK);
    }
}
