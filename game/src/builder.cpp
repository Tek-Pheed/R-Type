/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameBuilder
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <memory>
#include "Components.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameSystems.hpp"

void RType::GameInstance::createPersistentLevel()
{
    auto &level = refEntityManager.getPersistentLevel();

    if (!isServer())
        buildBackground();
    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::BackgroundSystem>().initSystem(*this);
}

ecs::Entity &RType::GameInstance::buildBackground()
{
    auto &bg = refEntityManager.getPersistentLevel().createEntity();
    auto &texture =
        refAssetManager.getAsset<sf::Texture>(Asset::BACKGROUND_TEXTURE);
    sf::Sprite sprite;

    texture.setRepeated(true);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::Rect(0, 0, 1280, 720));
    sprite.setTexture(
        refAssetManager.getAsset<sf::Texture>(Asset::BACKGROUND_TEXTURE));
    bg.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    bg.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 3.0, 3.0));
    bg.addComponent(std::make_shared<ecs::PositionComponent>(0, 0));
    bg.addComponent(std::make_shared<ecs::BackgroundComponent>(0.01f));
    return (bg);
}

void RType::GameInstance::levelMainMenu()
{
    auto &level = refEntityManager.createNewLevel("mainMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    refEntityManager.switchLevel("mainMenu");

    // To change later since this is supposed to be the main menu and not
    // contain any player
    //level.createSubsystem<GameSystems::BulletSystem>().initSystem(*this);
    //buildPlayer(true, 0);
}
