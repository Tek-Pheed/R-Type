/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RenderSystem.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Components.hpp"
#include "ErrorClass.hpp"
#include "Game.hpp"
#include "GameSystems.hpp"

using namespace RType;
using namespace GameSystems;

void RenderSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

void renderSprite(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto position = entity.getComponent<ecs::PositionComponent>();
    auto sprite = entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();

    if (!position || !sprite) {
        throw ErrorClass("RenderSprite failed to render entity, missing "
                         "component: position or sprite !");
        return;
    }
    sprite->getSprite().setPosition(
        position->getX() * (float) window.getSize().x,
        position->getY() * (float) window.getSize().y);
    window.draw(sprite->getSprite());
}

void renderSpriteAndText(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto position = entity.getComponent<ecs::PositionComponent>();
    auto sprite = entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!position || !sprite || !text) {
        throw ErrorClass(
            "RenderSpriteAndText failed to render entity, missing "
            "component: position or sprite or text !");
        return;
    }
    sprite->getSprite().setPosition(
        position->getX() * (float) window.getSize().x,
        position->getY() * (float) window.getSize().y);
    text->getText().setString(text->getStr());
    if (position->getY() <= 0.02) {
        text->getText().setPosition(
            position->getX() * (float) window.getSize().x
                + (float) sprite->getSprite().getTextureRect().width
                    * sprite->getSprite().getScale().x / 2
                - (float) text->getText().getLocalBounds().width / 2,
            position->getY() * (float) window.getSize().y
                + (float) sprite->getSprite().getTextureRect().height + 25);
    } else {
        text->getText().setPosition(
            position->getX() * (float) window.getSize().x
                + (float) sprite->getSprite().getTextureRect().width
                    * sprite->getSprite().getScale().x / 2
                - (float) text->getText().getLocalBounds().width / 2,
            position->getY() * (float) window.getSize().y - 15);
    }
    window.draw(sprite->getSprite());
    window.draw(text->getText());
}

void renderRectangle(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto rectangle =
        entity.getComponent<ecs::RectangleComponent<sf::RectangleShape>>();
    auto position = entity.getComponent<ecs::PositionComponent>();

    if (!rectangle || !position) {
        throw ErrorClass("RenderRectangle failed to render entity, missing "
                         "component: rectangle or position !");
        return;
    }
    rectangle->getRectangle().setPosition(position->getX(), position->getY());
    rectangle->getRectangle().setSize(sf::Vector2f(
        (float) rectangle->getSizeX(), (float) rectangle->getSizeY()));
    window.draw(rectangle->getRectangle());
}

void renderCircle(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto circle = entity.getComponent<ecs::CircleComponent<sf::CircleShape>>();
    auto position = entity.getComponent<ecs::PositionComponent>();

    if (!circle || !position) {
        throw ErrorClass("RenderCircle failed to render entity, missing "
                         "component: circle or position !");
        return;
    }
    circle->getCircle().setPosition(position->getX(), position->getY());
    circle->getCircle().setFillColor(sf::Color::Green);
    circle->getCircle().setRadius((float) circle->getRadius());
    window.draw(circle->getCircle());
}

void renderText(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto position = entity.getComponent<ecs::PositionComponent>();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!text || !position) {
        throw ErrorClass("RenderText failed to render entity, missing "
                         "component: text or position !");
        return;
    }
    text->getText().setPosition(position->getX(), position->getY());
    text->getText().setString(text->getStr());
    window.draw(text->getText());
}

void renderButton(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto rectangle =
        entity.getComponent<ecs::RectangleComponent<sf::RectangleShape>>();
    auto position = entity.getComponent<ecs::PositionComponent>();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!text || !position || !rectangle) {
        throw ErrorClass("RenderButton failed to render entity, missing "
                         "component: text or position or rectangle!");
        return;
    }
    rectangle->getRectangle().setPosition(position->getX(), position->getY());
    rectangle->getRectangle().setSize(sf::Vector2f(
        (float) rectangle->getSizeX(), (float) rectangle->getSizeY()));
    text->getText().setString(text->getStr());
    text->getText().setPosition(position->getX()
            + (float) rectangle->getSizeX() / 2
            - (float) text->getStr().length()
            - (float) text->getText().getLocalBounds().width / 2,
        position->getY() + (float) rectangle->getSizeY() / 2
            - (float) text->getText().getCharacterSize() / 2);
    window.draw(rectangle->getRectangle());
    window.draw(text->getText());
}

void renderInput(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto rectangle =
        entity.getComponent<ecs::RectangleComponent<sf::RectangleShape>>();
    auto position = entity.getComponent<ecs::PositionComponent>();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!text || !position || !rectangle) {
        throw ErrorClass("RenderInput failed to render entity, missing "
                         "component: text or position or rectangle!");
        return;
    }
    rectangle->getRectangle().setPosition(position->getX(), position->getY());
    rectangle->getRectangle().setSize(sf::Vector2f(
        (float) rectangle->getSizeX(), (float) rectangle->getSizeY()));
    text->getText().setString(text->getStr());
    text->getText().setPosition(position->getX()
            + (float) rectangle->getSizeX() / 2
            - (float) text->getStr().length()
            - (float) text->getText().getLocalBounds().width / 2,
        position->getY() + (float) rectangle->getSizeY() / 2
            - (float) text->getText().getCharacterSize() / 2);
    window.draw(rectangle->getRectangle());
    window.draw(text->getText());
}

void RenderSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    (void) deltaTime;

    for (auto &entity : entities) {
        auto renderComponent = entity.getComponent<ecs::RenderComponent>();

        if (!renderComponent)
            continue;
        switch (renderComponent->getObjectType()) {
            case ecs::RenderComponent::ObjectType::SPRITE:
                renderSprite(entity, _game->getWindow());
                break;
            case ecs::RenderComponent::ObjectType::SPRITEANDTEXT:
                renderSpriteAndText(entity, _game->getWindow());
                break;
            case ecs::RenderComponent::ObjectType::RECTANGLE:
                renderRectangle(entity, _game->getWindow());
                break;
            case ecs::RenderComponent::ObjectType::CIRCLE:
                renderCircle(entity, _game->getWindow());
                break;
            case ecs::RenderComponent::ObjectType::TEXT:
                renderText(entity, _game->getWindow());
                break;
            case ecs::RenderComponent::ObjectType::BUTTON:
                renderButton(entity, _game->getWindow());
                break;
            case ecs::RenderComponent::ObjectType::INPUT:
                renderInput(entity, _game->getWindow());
                break;
            default: break;
        }
    }
}
