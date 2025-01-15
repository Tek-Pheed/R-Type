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
        const float Width = 1.0f * (float) _game.WinScaleX;
        const float Height = 1.0f * (float) _game.WinScaleY;

        auto &bg = _game.refEntityManager.getPersistentLevel().createEntity();
        auto &texture = _game.refAssetManager.getAsset<sf::Texture>(
            Asset::BACKGROUND_TEXTURE);
        sf::Sprite sprite;

        sprite.setTexture(texture);
        sprite.setTextureRect(
            sf::Rect(0, 0, (int) GameInstance::DEFAULT_RESOLUTION_X,
                (int) GameInstance::DEFAULT_RESOLUTION_Y));
        sprite.setTexture(_game.refAssetManager.getAsset<sf::Texture>(
            Asset::BACKGROUND_TEXTURE));
        bg.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        bg.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 0));
        bg.addComponent(std::make_shared<ecs::PositionComponent>(0, 0));
        bg.addComponent(std::make_shared<ecs::BackgroundComponent>(0.01f));

        sprite.setScale(Width / sprite.getLocalBounds().width,
            Height / sprite.getLocalBounds().height);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
            sprite.getLocalBounds().height / 2.0f);
        texture.setRepeated(true);
        return (bg);
    }
} // namespace RType
