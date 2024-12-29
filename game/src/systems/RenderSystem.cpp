/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RenderSystem.cpp
*/

#include <iostream>
#include "Components.hpp"
#include "GameSystems.hpp"
#include "Game.hpp"

using namespace RType;
using namespace GameSystems;

void RenderSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

void RenderSystem::update(std::vector<ecs::Entity> &entities, float deltaTime)
{
    (void) deltaTime;

    _game->getWindow().clear();
    for (auto &entity : entities) {
        auto renderComponent = entity.getComponent<ecs::RenderComponent>();
        auto position = entity.getComponent<ecs::PositionComponent>();

        if (renderComponent->getObjectType()
            == ecs::RenderComponent::ObjectType::SPRITE) {
            auto sprite =
                entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();
            sprite->getSprite().setPosition(
                position->getX(), position->getY());
            _game->getWindow().draw(sprite->getSprite());
        } else if (renderComponent->getObjectType()
            == ecs::RenderComponent::ObjectType::RECTANGLE) {
            auto rectangle = entity.getComponent<
                ecs::RectangleComponent<sf::RectangleShape>>();
            rectangle->getRectangle().setPosition(
                position->getX(), position->getY());
            rectangle->getRectangle().setFillColor(sf::Color::Red);
            rectangle->getRectangle().setSize(sf::Vector2f(
                (float) rectangle->getSizeX(), (float) rectangle->getSizeY()));
            _game->getWindow().draw(rectangle->getRectangle());
        } else if (renderComponent->getObjectType()
            == ecs::RenderComponent::ObjectType::CIRCLE) {
            auto circle =
                entity.getComponent<ecs::CircleComponent<sf::CircleShape>>();
            circle->getCircle().setPosition(
                position->getX(), position->getY());
            circle->getCircle().setFillColor(sf::Color::Green);
            circle->getCircle().setRadius((float) circle->getRadius());
            _game->getWindow().draw(circle->getCircle());
        }
    }
    _game->getWindow().display();
}
