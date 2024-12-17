/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RectangleComponent.cpp
*/

#include "Components.hpp"

template <typename rectangleType>
ecs::RectangleComponent<rectangleType>::RectangleComponent(
    rectangleType rectangle, int sizeX, int sizeY)
{
    this->_rectangle = rectangle;
    this->_sizeY = sizeY;
    this->_sizeX = sizeX;
}

template <typename rectangleType>
int ecs::RectangleComponent<rectangleType>::getSizeX() const
{
    return this->_sizeX;
}

template <typename rectangleType>
int ecs::RectangleComponent<rectangleType>::getSizeY() const
{
    return this->_sizeY;
}

template <typename rectangleType>
rectangleType ecs::RectangleComponent<rectangleType>::getRectangle() const
{
    return this->_rectangle;
}

template <typename rectangleType>
void ecs::RectangleComponent<rectangleType>::setSizeX(int sizeX)
{
    this->_sizeX = sizeX;
}

template <typename rectangleType>
void ecs::RectangleComponent<rectangleType>::setSizeY(int sizeY)
{
    this->_sizeY = sizeY;
}

template <typename rectangleType>
void ecs::RectangleComponent<rectangleType>::setRectangle(
    rectangleType rectangle)
{
    this->_rectangle = rectangle;
}