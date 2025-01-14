/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BuildExplosion
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <memory>
#include <sstream>
#include "Components.hpp"
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "components/AIComponent.hpp"

using namespace RType;

ecs::Entity &Factory::buildExplosionEnemy(float posX, float posY)
{
    const float With = 0.05f * (float) _game.WindoScaleX;
    const float Height = With;

    auto &explosion = _game.refEntityManager.getCurrentLevel().createEntity();
    explosion.addComponent(
        std::make_shared<ecs::PositionComponent>(posX, posY));
    auto &texture =
        _game.refAssetManager.getAsset<sf::Texture>(Asset::EXPLOSION_TEXTURE);
    auto &explosionSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
        Asset::EXPLOSION_SOUND);

    buildSoundEffect(explosionSound, "explosionSound");

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::Rect(116, 17, 17, 17));
    sprite.setScale(With / sprite.getLocalBounds().width,
        Height / sprite.getLocalBounds().height);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
        sprite.getLocalBounds().height / 2.0f);
    explosion.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    explosion.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
        sprite, 17, 0, 116 + (17 * 5), 0.1f, 116, true));
    return explosion;
}

ecs::Entity &Factory::buildExplosionPlayer(float posX, float posY)
{
    const float With = 0.05f * (float) _game.WindoScaleX;
    const float Height = With;

    auto &explosion = _game.refEntityManager.getCurrentLevel().createEntity();
    explosion.addComponent(
        std::make_shared<ecs::PositionComponent>(posX, posY));
    auto &texture =
        _game.refAssetManager.getAsset<sf::Texture>(Asset::EXPLOSION_PLAYER);
    auto &explosionSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(
        Asset::EXPLOSION_SOUND);

    buildSoundEffect(explosionSound, "explosionSound");

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::Rect(168, 37, 33, 33));
    sprite.setScale(With / sprite.getLocalBounds().width,
        Height / sprite.getLocalBounds().height);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
        sprite.getLocalBounds().height / 2.0f);
    explosion.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    explosion.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
        sprite, 33, 0, 168 + (33 * 4), 0.1f, 168, true));
    return explosion;
}
