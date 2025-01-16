/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildInput.cpp
*/

#include "Factory.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"

using namespace RType;

ecs::Entity &Factory::buildInput(const std::string &str, int buttonID)
{
    auto &input = _game.refEntityManager.getCurrentLevel().createEntity();

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);
    rect.setSize(sf::Vector2f(700, 50));

    sf::Text text;
    text.setFont(_game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Black);
    text.setString(str);

    input.addComponent(std::make_shared<ecs::PositionComponent>(
        (float) _game.getWindow().getSize().x / 2
            - (float) rect.getSize().x / 2,
        (float) _game.getWindow().getSize().y / 2
            - (float) rect.getSize().y / 2 - (float) 75 * (float) buttonID));
    input.addComponent(
        std::make_shared<ecs::RectangleComponent<sf::RectangleShape>>(
            rect, rect.getSize().x, rect.getSize().y));
    input.addComponent(
        std::make_shared<ecs::TextComponent<sf::Text>>(text, str));
    input.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::INPUT));
    input.addComponent(
        std::make_shared<ecs::ClickableComponent>(ecs::ClickableType::INPUT));

    _game.inputList.push_back(input);

    return input;
}
