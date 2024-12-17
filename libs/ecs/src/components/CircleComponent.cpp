/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** CircleComponent.cpp
*/
#include "Components.hpp"

template <typename circleType>
ecs::CircleComponent<circleType>::CircleComponent(
    circleType circle, int radius)
{
    this->_circle = circle;
    this->_radius = radius;
}

template <typename circleType>
circleType ecs::CircleComponent<circleType>::getCircle() const
{
    return this->_circle;
}

template <typename circleType>
int ecs::CircleComponent<circleType>::getRadius() const
{
    return this->_circle;
}

template <typename circleType>
void ecs::CircleComponent<circleType>::setRadius(int radius)
{
    this->_radius = radius;
}

template <typename circleType>
void ecs::CircleComponent<circleType>::setCircle(circleType circle)
{
    this->_circle = circle;
}