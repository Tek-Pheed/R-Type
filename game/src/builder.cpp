/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameBuilder
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Window.hpp>
#include <iostream>
#include <map>
#include <memory>
#include "Components.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameSystems.hpp"

std::string getKeyString(sf::Keyboard::Key key)
{
    static const std::map<sf::Keyboard::Key, std::string> specialKeys = {
        {sf::Keyboard::Up, "Up"}, {sf::Keyboard::Down, "Down"},
        {sf::Keyboard::Left, "Left"}, {sf::Keyboard::Right, "Right"},
        {sf::Keyboard::Escape, "Escape"}, {sf::Keyboard::Space, "Space"},
        {sf::Keyboard::Return, "Return"}, {sf::Keyboard::Tab, "Tab"},
        {sf::Keyboard::BackSpace, "BackSpace"},
        {sf::Keyboard::Delete, "Delete"}, {sf::Keyboard::Home, "Home"},
        {sf::Keyboard::End, "End"}, {sf::Keyboard::PageUp, "PageUp"},
        {sf::Keyboard::PageDown, "PageDown"}, {sf::Keyboard::Period, "."}};

    auto it = specialKeys.find(key);

    if (it != specialKeys.end()) {
        return it->second;
    }
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z) {
        return std::string(
            1, static_cast<char>('A' + (key - sf::Keyboard::A)));
    }
    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9) {
        return std::to_string(key - sf::Keyboard::Num0);
    }
    if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9) {
        return std::to_string(key - sf::Keyboard::Numpad0);
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
        factory.buildBackground();
    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::BackgroundSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::HealthSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::HitboxSystem>().initSystem(*this);
    auto &musicSong =
        this->refAssetManager.getAsset<sf::SoundBuffer>(Asset::MENU_SONG);
    factory.buildMusic(musicSong, "menuSong");
}

void RType::GameInstance::handleAutoFireButton(
    std::string newAutoFireValue, ecs::Entity &entity)
{
    std::string value = (newAutoFireValue == "true" ? "YES" : "NO");
    gameConfig.updateConfigValue("AUTO_FIRE=", newAutoFireValue);
    gameConfig.saveConfig();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    text->setStr("AUTO FIRE : " + value);
}

void RType::GameInstance::handleResolutionButton(ecs::Entity &entity)
{
    static const std::vector<std::string> resolutions = {"1280x720",
        "1600x900", "1920x1080", "2560x1440", "2880x1620", "3840x2160"};

    std::string currentResolution = gameConfig.getConfig().at(6);
    size_t equalPos = currentResolution.find("=");
    if (equalPos != std::string::npos)
        currentResolution = currentResolution.substr(equalPos + 1);

    auto it =
        std::find(resolutions.begin(), resolutions.end(), currentResolution);
    std::string newResolution =
        (it != resolutions.end() && it + 1 != resolutions.end())
        ? *(it + 1)
        : resolutions.front();

    gameConfig.updateConfigValue("RESOLUTION=", newResolution);
    gameConfig.saveConfig();

    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    text->setStr("RESOLUTION : " + newResolution);
}

void RType::GameInstance::handleInputButtons(
    const std::vector<sf::Keyboard::Key> &keys)
{
    std::string nickname;
    const size_t maxInputLenght = 16;

    for (auto key : keys) {
        std::string keyStr = getKeyString(key);
        if (key == sf::Keyboard::BackSpace && !nickname.empty()) {
            nickname.pop_back();
        } else if (key != sf::Keyboard::BackSpace && keyStr.size() == 1) {
            if (nickname.size() < maxInputLenght) {
                nickname += keyStr;
            }
        }
    }

    for (auto &entity : refEntityManager.getCurrentLevel().getEntities()) {
        auto text = entity.get().getComponent<ecs::TextComponent<sf::Text>>();

        if (text && entity.get().getID() == lastInputIdClicked) {
            if (nickname.empty()) {
                text->setStr("");
            } else {
                text->setStr(nickname);
            }
            break;
        }
    }
}

void RType::GameInstance::handleConfigButtons(
    sf::Keyboard::Key pressedKey, int actionType)
{
    if (!isServer()) {
        std::string pressedKeyString = getKeyString(pressedKey);
        std::string newValue = "sf::Keyboard::" + pressedKeyString;

        switch (actionType) {
            case 0: {
                std::string moveUpAction = gameConfig.getConfig().at(1);
                size_t equalPosUp = moveUpAction.find("=");
                if (equalPosUp != std::string::npos) {
                    moveUpAction = moveUpAction.substr(equalPosUp + 1);
                }
                gameConfig.updateConfigValue("MOVE_UP=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == lastButtonIdClicked) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        text->setStr("MOVE UP : " + pressedKeyString);
                        break;
                    }
                }
                break;
            }
            case -1: {
                std::string moveRightAction = gameConfig.getConfig().at(2);
                size_t equalPosRight = moveRightAction.find("=");
                if (equalPosRight != std::string::npos) {
                    moveRightAction =
                        moveRightAction.substr(equalPosRight + 1);
                }
                gameConfig.updateConfigValue("MOVE_RIGHT=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == lastButtonIdClicked) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        text->setStr("MOVE RIGHT : " + pressedKeyString);
                        break;
                    }
                }
                break;
            }
            case -2: {
                std::string moveLeftAction = gameConfig.getConfig().at(3);
                size_t equalPosLeft = moveLeftAction.find("=");
                if (equalPosLeft != std::string::npos) {
                    moveLeftAction = moveLeftAction.substr(equalPosLeft + 1);
                }
                gameConfig.updateConfigValue("MOVE_LEFT=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == lastButtonIdClicked) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        text->setStr("MOVE LEFT : " + pressedKeyString);
                        break;
                    }
                }
                break;
            }
            case -3: {
                std::string moveDownAction = gameConfig.getConfig().at(4);
                size_t equalPosDown = moveDownAction.find("=");
                if (equalPosDown != std::string::npos) {
                    moveDownAction = moveDownAction.substr(equalPosDown + 1);
                }
                gameConfig.updateConfigValue("MOVE_DOWN=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == lastButtonIdClicked) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        text->setStr("MOVE DOWN : " + pressedKeyString);
                        break;
                    }
                }
                break;
            }
            default: std::cerr << "Invalid action type." << std::endl; break;
        }
        gameConfig.saveConfig();
    }
}
