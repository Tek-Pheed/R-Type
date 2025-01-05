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
#include <SFML/Window.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <map>
#include "Components.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameSystems.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "GameProtocol.hpp"
#include "system_network.hpp"
#include "Config.hpp"

std::string getKeyString(sf::Keyboard::Key key) {
    static const std::map<sf::Keyboard::Key, std::string> specialKeys = {
        {sf::Keyboard::Up, "Up"},
        {sf::Keyboard::Down, "Down"},
        {sf::Keyboard::Left, "Left"},
        {sf::Keyboard::Right, "Right"},
        {sf::Keyboard::Escape, "Escape"},
        {sf::Keyboard::Space, "Space"},
        {sf::Keyboard::Return, "Return"},
        {sf::Keyboard::Tab, "Tab"},
        {sf::Keyboard::BackSpace, "BackSpace"},
        {sf::Keyboard::Delete, "Delete"},
        {sf::Keyboard::Home, "Home"},
        {sf::Keyboard::End, "End"},
        {sf::Keyboard::PageUp, "PageUp"},
        {sf::Keyboard::PageDown, "PageDown"}
    };

    auto it = specialKeys.find(key);

    if (it != specialKeys.end()) {
        return it->second;
    }
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z) {
        return std::string(1, static_cast<char>('A' + (key - sf::Keyboard::A)));
    }
    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9) {
        return std::to_string(key - sf::Keyboard::Num0);
    }
    if (key >= sf::Keyboard::F1 && key <= sf::Keyboard::F15) {
        return std::to_string(1 + (key - sf::Keyboard::F1));
    }
    return "sf::Keyboard::Unknown";
}

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

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(2);
    rect.setSize(sf::Vector2f(700, 50));

    sf::Text text;
    text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Black);
    text.setString(str);

    button.addComponent(std::make_shared<ecs::PositionComponent>((float) this->_window->getSize().x / 2 - (float) rect.getSize().x / 2,(float) this->_window->getSize().y / 2 - (float) rect.getSize().y / 2 - (float) 75 * (float) buttonID));
    button.addComponent(std::make_shared<ecs::RectangleComponent<sf::RectangleShape>>(rect, rect.getSize().x, rect.getSize().y));
    button.addComponent(std::make_shared<ecs::TextComponent<sf::Text>>(text, str));
    button.addComponent(std::make_shared<ecs::RenderComponent>(ecs::RenderComponent::ObjectType::BUTTON));

    return button;
}

void RType::GameInstance::levelMainMenu()
{
    auto &level = refEntityManager.createNewLevel("mainMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    refEntityManager.switchLevel("mainMenu");

    if (!isServer()) {
        auto &title = refEntityManager.getCurrentLevel().createEntity();

        sf::Text text;
        text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::White);
        text.setString("F Type V8");

        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float)this->_window->getSize().x;

        float posX = (windowWidth - textWidth) / 2;
        float posY = (float)this->_window->getSize().y / 4;

        title.addComponent(std::make_shared<ecs::RenderComponent>(ecs::RenderComponent::ObjectType::TEXT));
        title.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
        title.addComponent(std::make_shared<ecs::TextComponent<sf::Text>>(text, "F Type V8"));

        buildButton("PLAY", 1);
        buildButton("SETTINGS", 0);
        buildButton("EXIT", -1);
    }
}

void RType::GameInstance::levelSettingsMenu()
{
    Config config("config.cfg");
    auto &level = refEntityManager.createNewLevel("settingsMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    refEntityManager.switchLevel("settingsMenu");

    if (!isServer()) {
        auto &title = refEntityManager.getCurrentLevel().createEntity();

        sf::Text text;
        text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::White);
        text.setString("SETTINGS");

        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float)this->_window->getSize().x;

        float posX = (windowWidth - textWidth) / 2;
        float posY = (float)this->_window->getSize().y / 4;

        title.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT));
        title.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
        title.addComponent(std::make_shared<ecs::TextComponent<sf::Text>>(text, "SETTINGS"));

        std::string moveUpAction = config.getConfig().at(1);
        std::string moveRightAction = config.getConfig().at(2);
        std::string moveLeftAction = config.getConfig().at(3);
        std::string moveDownAction = config.getConfig().at(4);
        std::string autoFireAction = config.getConfig().at(7);

        std::string endUpMove = "";
        std::string endRightMove = "";
        std::string endLeftMove = "";
        std::string endDownMove = "";
        std::string endAutoFire = "";

        size_t equalPosUp = moveUpAction.find("=");
        if (equalPosUp != std::string::npos) {
            std::string beforeUpEqual = moveUpAction.substr(0, equalPosUp);
            std::string afterUpEqual = moveUpAction.substr(equalPosUp + 1);
            endUpMove = beforeUpEqual + " : " + afterUpEqual;
        }

        size_t equalPosRight = moveRightAction.find("=");
        if (equalPosRight != std::string::npos) {
            std::string beforeRightEqual = moveRightAction.substr(0, equalPosRight);
            std::string afterRightEqual = moveRightAction.substr(equalPosRight + 1);
            endRightMove = beforeRightEqual + " : " + afterRightEqual;
        }

        size_t equalPosLeft = moveLeftAction.find("=");
        if (equalPosLeft != std::string::npos) {
            std::string beforeLeftEqual = moveLeftAction.substr(0, equalPosLeft);
            std::string afterLeftEqual = moveLeftAction.substr(equalPosLeft + 1);
            endLeftMove = beforeLeftEqual + " : " + afterLeftEqual;
        }

        size_t equalPosDown = moveDownAction.find("=");
        if (equalPosDown != std::string::npos) {
            std::string beforeDownEqual = moveDownAction.substr(0, equalPosDown);
            std::string afterDownEqual = moveDownAction.substr(equalPosDown + 1);
            endDownMove = beforeDownEqual + " : " + afterDownEqual;
        }

        size_t equalPosAutoFire = autoFireAction.find("=");
        if (equalPosAutoFire != std::string::npos) {
            std::string beforeAutoFireEqual = "Auto fire";
            std::string afterAutoFireEqual = autoFireAction.substr(equalPosAutoFire + 1);
            endAutoFire = beforeAutoFireEqual + " : " + (afterAutoFireEqual == "true" ? "Yes" : "No");
        }

        buildButton(endUpMove, 2);
        buildButton(endRightMove, 1);
        buildButton(endLeftMove, 0);
        buildButton(endDownMove, -1);
        buildButton(endAutoFire, -2);
        buildButton("BACK", -3);
    }
}

void RType::GameInstance::handleAutoFireButton(std::string newAutoFireValue, ecs::Entity &entity)
{
    Config config("config.cfg");
    std::string value = (newAutoFireValue == "true" ? "Yes" : "No");
    config.updateConfigValue("AUTO_FIRE=", newAutoFireValue);
    config.saveConfig();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    text->setStr("Auto fire : " + value);
}

void RType::GameInstance::handleConfigButtons(sf::Keyboard::Key pressedKey, int actionType, ecs::Entity &entity)
{
    if (!isServer()) {
        Config config("config.cfg");
        std::string pressedKeyString = getKeyString(pressedKey);
        std::string newValue = "sf::Keyboard::" + pressedKeyString;

        switch (actionType) {
            case 0: {
                std::string moveUpAction = config.getConfig().at(1);
                size_t equalPosUp = moveUpAction.find("=");
                if (equalPosUp != std::string::npos) {
                    moveUpAction = moveUpAction.substr(equalPosUp + 1);
                }
                config.updateConfigValue("MOVE_UP=", newValue);
                auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
                text->setStr("MOVE_UP :" +  pressedKeyString);
                break;
            }
            case -1: {
                std::string moveRightAction = config.getConfig().at(2);
                size_t equalPosRight = moveRightAction.find("=");
                if (equalPosRight != std::string::npos) {
                    moveRightAction = moveRightAction.substr(equalPosRight + 1);
                }
                config.updateConfigValue("MOVE_RIGHT=", newValue);
                auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
                text->setStr("MOVE_RIGHT :" +  pressedKeyString);
                break;
            }
            case -2: {
                std::string moveLeftAction = config.getConfig().at(3);
                size_t equalPosLeft = moveLeftAction.find("=");
                if (equalPosLeft != std::string::npos) {
                    moveLeftAction = moveLeftAction.substr(equalPosLeft + 1);
                }
                config.updateConfigValue("MOVE_LEFT=", newValue);
                auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
                text->setStr("MOVE_LEFT :" +  pressedKeyString);
                break;
            }
            case -3: {
                std::string moveDownAction = config.getConfig().at(4);
                size_t equalPosDown = moveDownAction.find("=");
                if (equalPosDown != std::string::npos) {
                    moveDownAction = moveDownAction.substr(equalPosDown + 1);
                }
                config.updateConfigValue("MOVE_DOWN=", newValue);
                auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
                text->setStr("MOVE_DOWN :" +  pressedKeyString);
                break;
            }
            default:
                std::cerr << "Invalid action type." << std::endl;
                break;
        }
        config.saveConfig();
    }
}
