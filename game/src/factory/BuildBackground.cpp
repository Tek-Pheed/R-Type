/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BuildBackground.cpp
*/

#include "Factory.hpp"
#include "GameAssets.hpp"

namespace RType
{
    ecs::Entity &Factory::buildBackground()
    {
        auto &bg = _game.refEntityManager.getPersistentLevel().createEntity();
        auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
            Asset::BACKGROUND_TEXTURE);
        sf::Sprite sprite;

        texture.setRepeated(true);
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 0, (int) GameInstance::RESOLUTION_X,
            (int) GameInstance::RESOLUTION_Y));
        sprite.setTexture(_game.refAssetManager.getAsset<sf::Texture>(
            Asset::BACKGROUND_TEXTURE));
        bg.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        bg.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 3.0, 3.0));
        bg.addComponent(std::make_shared<ecs::PositionComponent>(0, 0));
        bg.addComponent(std::make_shared<ecs::BackgroundComponent>(0.01f));
        return (bg);
    }
} // namespace RType
