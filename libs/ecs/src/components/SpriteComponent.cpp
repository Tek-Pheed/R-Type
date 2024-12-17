/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SpriteComponent.cpp
*/

#include "Components.hpp"

template <typename spriteType>
ecs::SpriteComponent<spriteType>::SpriteComponent(
    spriteType sprite, int sizeX, int sizeY)
{
    this->_sprite = sprite;
    this->_sizeX = sizeX;
    this->_sizeY = sizeY;
}