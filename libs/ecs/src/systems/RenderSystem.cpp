/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RenderSystem.cpp
*/

#include "Components.hpp"
#include "Systems.hpp"

void ecs::RenderSystem::update(
    std::vector<std::shared_ptr<ecs::Entity>> &entities,
    sf::RenderWindow *window, float deltaTime)
{
    (void) deltaTime;

    for (auto &entity : entities) {
        auto renderComponent = entity->getComponent<RenderComponent>();
        auto position = entity->getComponent<PositionComponent>();
        auto player = entity->getComponent<PlayerComponent>();

        if (renderComponent->getObjectType() == ecs::ObjectType::SPRITE) {
            renderComponent->getSprite()->setPosition(
                position->getX(), position->getY());
            window->draw(*(renderComponent->getSprite()));
        } else if (renderComponent->getObjectType()
            == ecs::ObjectType::RECTANGLE) {
            renderComponent->getRectangleShape()->setPosition(
                position->getX(), position->getY());
            renderComponent->getRectangleShape()->setFillColor(sf::Color::Red);
            renderComponent->getRectangleShape()->setSize(
                sf::Vector2f(100, 100));
            window->draw(*(renderComponent->getRectangleShape()));
        } else if (renderComponent->getObjectType()
            == ecs::ObjectType::CIRCLE) {
            renderComponent->getCircleShape()->setPosition(
                position->getX(), position->getY());
            renderComponent->getCircleShape()->setFillColor(sf::Color::Yellow);
            renderComponent->getCircleShape()->setRadius(100);
            window->draw(*(renderComponent->getCircleShape()));
        }
    }
}
