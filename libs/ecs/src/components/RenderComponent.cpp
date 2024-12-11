/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** RenderComponent
*/

#include "Components.hpp"

ecs::RenderComponent::RenderComponent(
    ObjectType type, const sf::Texture &texture, sf::Vector2f position)
{
    this->_type = type;
    if (type == ecs::ObjectType::SPRITE) {
        this->sprite = new sf::Sprite(texture);
        this->sprite->setPosition(position);
    } else if (type == ecs::ObjectType::CIRCLE) {
        this->circleShape = new sf::CircleShape();
        this->circleShape->setPosition(position);
    } else if (type == ecs::ObjectType::RECTANGLE) {
        this->rectangleShape = new sf::RectangleShape();
        this->rectangleShape->setPosition(position);
    }
}

ecs::ObjectType ecs::RenderComponent::getObjectType() const
{
    return this->_type;
}

void ecs::RenderComponent::setObjectType(ecs::ObjectType type)
{
    this->_type = type;
}

sf::Sprite *ecs::RenderComponent::getSprite()
{
    return this->sprite;
}

sf::RectangleShape *ecs::RenderComponent::getRectangleShape()
{
    return this->rectangleShape;
}

sf::CircleShape *ecs::RenderComponent::getCircleShape()
{
    return this->circleShape;
}