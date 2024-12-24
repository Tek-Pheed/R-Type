/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RenderSystem.cpp
*/

#include "Components.hpp"
#include "Systems.hpp"

void ecs::RenderSystem::update(std::vector<ecs::Entity> &entities,
    sf::RenderWindow &window, float deltaTime, bool isServer)
{
    (void) deltaTime;
    (void) isServer;

    for (auto &entity : entities) {
        auto renderComponent = entity.getComponent<ecs::RenderComponent>();
        auto position = entity.getComponent<ecs::PositionComponent>();

        if (renderComponent->getObjectType() == ecs::ObjectType::SPRITE) {
            auto sprite =
                entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();
            sprite->getSprite().setPosition(
                position->getX(), position->getY());
            window.draw(sprite->getSprite());
        } else if (renderComponent->getObjectType()
            == ecs::ObjectType::RECTANGLE) {
            auto rectangle = entity.getComponent<
                ecs::RectangleComponent<sf::RectangleShape>>();
            rectangle->getRectangle().setPosition(
                position->getX(), position->getY());
            rectangle->getRectangle().setFillColor(sf::Color::Red);
            rectangle->getRectangle().setSize(sf::Vector2f(
                (float) rectangle->getSizeX(), (float) rectangle->getSizeY()));
            window.draw(rectangle->getRectangle());
        } else if (renderComponent->getObjectType()
            == ecs::ObjectType::CIRCLE) {
            auto circle =
                entity.getComponent<ecs::CircleComponent<sf::CircleShape>>();
            circle->getCircle().setPosition(
                position->getX(), position->getY());
            circle->getCircle().setFillColor(sf::Color::Green);
            circle->getCircle().setRadius((float) circle->getRadius());
            window.draw(circle->getCircle());
        }
    }
}
