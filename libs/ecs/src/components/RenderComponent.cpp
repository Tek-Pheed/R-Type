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
        sf::Sprite sprite1(texture);
        this->sprite = &sprite1;
        this->sprite->setPosition(position);
    } else if (type == ecs::ObjectType::CIRCLE) {
        sf::CircleShape circleShape1;
        this->circleShape = &circleShape1;
        this->circleShape->setPosition(position);
    } else if (type == ecs::ObjectType::RECTANGLE) {
        sf::RectangleShape rect;
        this->rectangleShape = &rect;
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

sf::Sprite ecs::RenderComponent::getSprite() const
{
    return *this->sprite;
}

sf::RectangleShape ecs::RenderComponent::getRectangleShape() const
{
    return *this->rectangleShape;
}

sf::CircleShape ecs::RenderComponent::getCircleShape() const
{
    return *this->circleShape;
}