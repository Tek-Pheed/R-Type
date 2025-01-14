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

void renderSprite(ecs::Entity &entity, sf::RenderWindow &window,
    float deltaTime, GameInstance &game)
{
    auto position = entity.getComponent<ecs::PositionComponent>();
    auto sprite = entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();
    float elapsedTime = 0.0f;

    if (!position || !sprite) {
        throw ErrorClass(THROW_ERROR_LOCATION
            "RenderSprite failed to render entity, missing "
            "component: position or sprite !");
        return;
    }

    elapsedTime = sprite->getElapsedTime() + deltaTime;
    if (sprite->getSizeX() != 0) {
        if (elapsedTime >= sprite->getDelay()) {
            auto textureRect = sprite->getSprite().getTextureRect();
            textureRect.left += sprite->getSizeX();
            if (textureRect.left >= sprite->getMaxX()) {
                if (sprite->getOnce()) {
                    game.refEntityManager.getCurrentLevel()
                        .markEntityForDeletion(entity.getID());
                } else
                    textureRect.left = sprite->getStartX();
            }
            sprite->getSprite().setTextureRect(textureRect);
            elapsedTime -= sprite->getDelay();
        }
    }
    sprite->setElapsedTime(elapsedTime);
    sprite->getSprite().setPosition(position->getX(), position->getY());
    window.draw(sprite->getSprite());
}

void renderSpriteAndText(ecs::Entity &entity, sf::RenderWindow &window)
{
    auto position = entity.getComponent<ecs::PositionComponent>();
    auto sprite = entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();

    if (!position || !sprite || !text) {
        throw ErrorClass(THROW_ERROR_LOCATION
            "RenderSpriteAndText failed to render entity, missing "
            "component: position or sprite or text !");
        return;
    }
    sprite->getSprite().setPosition(position->getX(), position->getY());
    text->getText().setString(text->getStr());
    if (position->getY() < 15) {
        text->getText().setPosition(position->getX()
                + (float) sprite->getSprite().getTextureRect().width
                    * sprite->getSprite().getScale().x / 2
                - (float) text->getText().getLocalBounds().width / 2,
            position->getY()
                + static_cast<float>(
                    sprite->getSprite().getTextureRect().height)
                + 15);
    } else {
        text->getText().setPosition(position->getX()
                + (float) sprite->getSprite().getTextureRect().width
                    * sprite->getSprite().getScale().x / 2
                - (float) text->getText().getLocalBounds().width / 2,
            position->getY() - 30);
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
        throw ErrorClass(THROW_ERROR_LOCATION
            "RenderRectangle failed to render entity, missing "
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
        throw ErrorClass(THROW_ERROR_LOCATION
            "RenderCircle failed to render entity, missing "
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
        throw ErrorClass(
            THROW_ERROR_LOCATION "RenderText failed to render entity, missing "
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
        throw ErrorClass(THROW_ERROR_LOCATION
            "RenderButton failed to render entity, missing "
            "component: text or position or rectangle!");
        return;
    }
    rectangle->getRectangle().setPosition(position->getX(), position->getY());
    rectangle->getRectangle().setSize(sf::Vector2f(
        (float) rectangle->getSizeX(), (float) rectangle->getSizeY()));
    text->getText().setString(text->getStr());
    text->getText().setPosition(position->getX()
            + (float) rectangle->getSizeX() / 2
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
        throw ErrorClass(THROW_ERROR_LOCATION
            "RenderInput failed to render entity, missing "
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
    for (auto &entity : entities) {
        auto renderComponent = entity.getComponent<ecs::RenderComponent>();

        if (!renderComponent)
            continue;
        switch (renderComponent->getObjectType()) {
            case ecs::RenderComponent::ObjectType::SPRITE:
                if (entity.getComponent<ecs::EnemyComponent>()) {
                    if (entity.getComponent<ecs::EnemyComponent>()->getWave()
                        == _game->currentWave)
                        renderSprite(entity, _game->getWindow(), deltaTime);
                } else {
                    renderSprite(entity, _game->getWindow(), deltaTime);
                }
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
