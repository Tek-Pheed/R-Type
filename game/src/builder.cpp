/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameBuilder
*/

#include <memory>
#include "Components.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameBuilder.hpp"
#include "GameSystems.hpp"

void RType::createPersistentLevel(GameInstance &game)
{
    auto &level = game.entityManager.getPersistentLevel();

    if (!game.isServer())
        RType::buildBackground(game);
    level.createSubsystem<GameSystems::RenderSystem>().initSystem(game);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(game);
}

ecs::Entity &RType::buildBackground(RType::GameInstance &game)
{
    auto &bg = game.entityManager.getPersistentLevel().createEntity();
    auto &texture =
        game.assetManager.getAsset<sf::Texture>(Asset::BACKGROUND_TEXTURE);
    sf::Sprite sprite;

    texture.setRepeated(true);
    sprite.setTextureRect(sf::Rect(0, 0, 1280, 720));
    sprite.setTexture(texture);
    bg.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    bg.addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    bg.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 3.0, 3.0));
    return (bg);
}

void RType::buildMainMenu(RType::GameInstance &game)
{
    auto &level = game.entityManager.createNewLevel("mainMenu");
    auto &bg = level.createEntity();
    auto &texture =
        game.assetManager.getAsset<sf::Texture>(Asset::BACKGROUND_TEXTURE);
    sf::Sprite sprite;

    sprite.setTextureRect(sf::Rect(0, 0, 1280, 720));
    sprite.setTexture(texture);
    bg.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    bg.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 3.0, 3.0));
    bg.addComponent(std::make_shared<ecs::PositionComponent>(0, 0));

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(game);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(game);
    game.entityManager.switchLevel("mainMenu");
}

ecs::Entity &RType::buildPlayer(
    RType::GameInstance &game, const std::string &levelName)
{
    auto &player =
        game.entityManager.getSpecificLevel(levelName).createEntity();
    auto &texture =
        game.assetManager.getAsset<sf::Texture>(Asset::PLAYER_TEXTURE);
    sf::Sprite sprite;

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::Rect(66, 0, 33, 14));
    sprite.setScale(sf::Vector2f(3, 3));
    player.addComponent(std::make_shared<ecs::PlayerComponent>());
    player.addComponent(std::make_shared<ecs::PositionComponent>(100, 100));
    player.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::SPRITE));
    player.addComponent(std::make_shared<ecs::VelocityComponent>(0, 0));
    player.addComponent(std::make_shared<ecs::HealthComponent>(100));
    player.addComponent(
        std::make_shared<ecs::SpriteComponent<sf::Sprite>>(sprite, 3.0, 3.0));
    game.setPlayerId((int) player.getID());
    return (player);
}
