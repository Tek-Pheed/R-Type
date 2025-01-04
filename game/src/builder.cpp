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
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "GameProtocol.hpp"
#include "system_network.hpp"

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

ecs::Entity &RType::GameInstance::buildButton(std::string str, int buttonID)
{
    auto &button = refEntityManager.getCurrentLevel().createEntity();
    button.addComponent(std::make_shared<ecs::PositionComponent>(this->_window->getSize().x / 2 - 150, this->_window->getSize().y / 2 - 25 - 75 * buttonID));

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(2);

    sf::Text text;
    text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    button.addComponent(std::make_shared<ecs::RectangleComponent<sf::RectangleShape>>(rect, 300, 50));
    button.addComponent(std::make_shared<ecs::TextComponent<sf::Text>>(text, str));
    button.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::BUTTON));

    return button;
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

    if (!isServer()) {
        auto &title = refEntityManager.getCurrentLevel().createEntity();

        sf::Text text;
        text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);

        title.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT));
        title.addComponent(std::make_shared<ecs::PositionComponent>(this->_window->getSize().x / 2 - 9 * text.getCharacterSize() / 2, this->_window->getSize().y / 4));
        title.addComponent(std::make_shared<ecs::TextComponent<sf::Text>>(text, "F Type V8"));

        buildButton("Play", 1);
        buildButton("Settings", 0);
        buildButton("Exit", -1);
    }
}
