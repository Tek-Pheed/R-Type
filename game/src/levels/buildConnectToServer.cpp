/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** buildConnectToServer.cpp
*/

#include "Game.hpp"
#include "GameAssets.hpp"
#include "Levels.hpp"

using namespace RType;

void Levels::buildConnectToServerMenu()
{
    auto &level = _game.refEntityManager.createNewLevel("continueMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(_game);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(_game);
    _game.refEntityManager.switchLevel("continueMenu");

    if (!_game.isServer()) {
        auto &title = _game.refEntityManager.getCurrentLevel().createEntity();

        sf::Text text;
        text.setFont(
            _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.0f);
        text.setString("CONNECTION TO SERVER");

        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float) _game.getWindow().getSize().x;

        float posX = (windowWidth - textWidth) / 2;
        float posY = (float) _game.getWindow().getSize().y / 10;

        title.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT));
        title.addComponent(
            std::make_shared<ecs::PositionComponent>(posX, posY));
        title.addComponent(std::make_shared<ecs::TextComponent<sf::Text>>(
            text, "CONNECTION TO SERVER"));

        _game._factory.buildInput("NICKNAME", 2);
        _game._factory.buildInput("IP ADDRESS", 1);
        _game._factory.buildInput("8081", 0);
        _game._factory.buildInput("8082", -1);
        _game._factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -3),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            "PLAY GAME", 40, sf::Color::Black, ecs::ClickableType::LOBBY);

        _game._factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -4),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "BACK",
            40, sf::Color::Black, ecs::ClickableType::BACK);
    }
}