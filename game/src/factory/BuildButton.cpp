/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** buildButton.cpp
*/

#include "Factory.hpp"
#include "GameAssets.hpp"

namespace RType
{
    ecs::Entity &Factory::buildButton(sf::Vector2f position, sf::Vector2f size,
        sf::Color fillColor, sf::Color outlineColor, const std::string &str,
        unsigned int characterSize, sf::Color textColor,
        ecs::ClickableType type)
    {
        auto &button =
            _game->refEntityManager.getCurrentLevel().createEntity();

        sf::RectangleShape rect;
        rect.setFillColor(fillColor);
        rect.setOutlineColor(outlineColor);
        rect.setOutlineThickness(2);
        rect.setSize(size);

        sf::Text text;
        text.setFont(
            _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(characterSize);
        text.setFillColor(textColor);
        text.setString(str);

        button.addComponent(
            std::make_shared<ecs::PositionComponent>(position.x, position.y));
        button.addComponent(
            std::make_shared<ecs::RectangleComponent<sf::RectangleShape>>(
                rect, size.x, size.y));
        button.addComponent(
            std::make_shared<ecs::TextComponent<sf::Text>>(text, str));
        button.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::BUTTON));
        button.addComponent(std::make_shared<ecs::ClickableComponent>(type));

        _game->_inputList.push_back(button);

        return button;
    }
} // namespace RType