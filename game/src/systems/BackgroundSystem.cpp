/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BackgroundSystem.cpp
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Components.hpp"
#include "Game.hpp"
#include "GameSystems.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

using namespace RType;
using namespace GameSystems;

void BackgroundSystem::initSystem(GameInstance &gameRef)
{
    _game = &gameRef;
}

void BackgroundSystem::update(
    std::vector<ecs::Entity> &entities, float deltaTime)
{
    if (_game->isServer())
        return;
    for (auto &entity : entities) {
        auto bgComp = entity.getComponent<ecs::BackgroundComponent>();
        auto sprite = entity.getComponent<ecs::SpriteComponent<sf::Sprite>>();
        if (!bgComp || !sprite)
            continue;
        bgComp->setClock(bgComp->getClock() + deltaTime);
        if (bgComp->isTimeToMove()) {
            sprite->getSprite().setTextureRect(
                sf::Rect(sprite->getSprite().getTextureRect().left + 2, 0,
                    int(_game->getWindow().getSize().x),
                    int(_game->getWindow().getSize().y)));
            bgComp->setClock(0.0f);
        }
    }
}
