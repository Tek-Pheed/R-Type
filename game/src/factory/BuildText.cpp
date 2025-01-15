/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BuildText
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <memory>
#include <mutex>
#include <sstream>
#include "Components.hpp"
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"

using namespace RType;

ecs::Entity &RType::Factory::buildText(
    size_t id, float posX, float posY, std::string &content, sf::Color color, unsigned int characterSize)
{
    auto &text = _game.refEntityManager.getCurrentLevel().createEntity();
    if (!_game.isServer()) {
        text.addComponent(
            std::make_shared<ecs::PositionComponent>(posX, posY));
        text.setID(id);

        auto &font =
            _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT);
        sf::Text sfText;

        sfText.setFont(font);
        sfText.setCharacterSize(characterSize);
        sfText.setFillColor(color);
        sfText.setString(content);

        sf::FloatRect textRect = sfText.getLocalBounds();
        if (posX + textRect.width
            > static_cast<float>(_game.getWindow().getSize().x)) {
            posX = static_cast<float>(_game.getWindow().getSize().x)
                - textRect.width;
        }
        if (posY + textRect.height
            > static_cast<float>(_game.getWindow().getSize().y)) {
            posY = static_cast<float>(_game.getWindow().getSize().y)
                - textRect.height;
        }
        text.getComponent<ecs::PositionComponent>()->setX(posX);
        text.getComponent<ecs::PositionComponent>()->setY(posY);

        text.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT));
        text.addComponent(
            std::make_shared<ecs::TextComponent<sf::Text>>(sfText, content));
    }
    return (text);
}
