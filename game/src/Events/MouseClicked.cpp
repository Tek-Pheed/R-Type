/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MouseClicked.cpp
*/

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include "Components.hpp"
#include "Config.hpp"
#include "Entity.hpp"
#include "Events.hpp"
#include "GameProtocol.hpp"
#include "components/ClickableComponent.hpp"

using namespace RType;

void EventManager::handleStartButton(bool isHost)
{
    size_t id = _game.getLocalPlayer()
                    .getComponent<ecs::PlayerComponent>()
                    ->getPlayerID();
    std::stringstream ss;
    if (isHost && id != _game.getHostClient())
        return;
    if (isHost) {
        ss << Protocol::L_STARTGAME << " " << id << " " << PACKET_END;
        _game.refNetworkManager.sendToAll(
            System::Network::ISocket::Type::TCP, ss.str());
    }
}

void EventManager::handleDifficultyButton(ecs::Entity &entity, bool isHost)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    size_t id = _game.getLocalPlayer()
                    .getComponent<ecs::PlayerComponent>()
                    ->getPlayerID();

    if (!text)
        return;
    if (isHost && id != _game.getHostClient())
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string difficulty = str.substr(separator + 2);
        if (difficulty == "EASY") {
            text->setStr("DIFFICULTY : MEDIUM");
            ss << Protocol::L_SETDIFFICULTY << " " << id << " " << 2
               << PACKET_END;
        } else if (difficulty == "MEDIUM") {
            text->setStr("DIFFICULTY : HARD");
            ss << Protocol::L_SETDIFFICULTY << " " << id << " " << 3
               << PACKET_END;
        } else {
            text->setStr("DIFFICULTY : EASY");
            ss << Protocol::L_SETDIFFICULTY << " " << id << " " << 1
               << PACKET_END;
        }
        if (isHost)
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
    }
}

void EventManager::handleBonusButton(ecs::Entity &entity, bool isHost)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    size_t id = _game.getLocalPlayer()
                    .getComponent<ecs::PlayerComponent>()
                    ->getPlayerID();

    if (!text)
        return;
    if (isHost && id != _game.getHostClient())
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string bonus = str.substr(separator + 2);
        if (bonus == "YES") {
            text->setStr("BONUS : NO");
            ss << Protocol::L_SETBONUS << " " << id << " " << 0 << PACKET_END;
        } else {
            text->setStr("BONUS : YES");
            ss << Protocol::L_SETBONUS << " " << id << " " << 1 << PACKET_END;
        }
        if (isHost)
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
    }
}

void EventManager::handleLevelButton(ecs::Entity &entity, bool isHost)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    size_t id = _game.getLocalPlayer()
                    .getComponent<ecs::PlayerComponent>()
                    ->getPlayerID();

    if (!text)
        return;
    if (isHost && id != _game.getHostClient())
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string level = str.substr(separator + 2);
        int levelNB = std::atoi(level.c_str());

        if (levelNB >= _game._nbTxtFiles) {
            levelNB = 1;
            ss << Protocol::L_SETLEVEL << " " << id << " " << 1 << PACKET_END;
        } else {
            levelNB++;
            ss << Protocol::L_SETLEVEL << " " << id << " " << levelNB
               << PACKET_END;
        }
        text->setStr("LEVEL : " + std::to_string(levelNB));
        if (isHost)
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
    }
}

void EventManager::handleNumberOfPlayerButton(ecs::Entity &entity, bool isHost)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    size_t id = _game.getLocalPlayer()
                    .getComponent<ecs::PlayerComponent>()
                    ->getPlayerID();

    if (!text)
        return;
    if (isHost && id != _game.getHostClient())
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string maxPlayer = str.substr(separator + 1);
        int maxPlayerNB = std::atoi(maxPlayer.c_str());
        if (maxPlayerNB >= 10) {
            maxPlayerNB = 4;
            ss << Protocol::L_SETMAXPLAYRS << " " << id << " " << 4
               << PACKET_END;
        } else {
            maxPlayerNB++;
            ss << Protocol::L_SETMAXPLAYRS << " " << id << " " << maxPlayerNB
               << PACKET_END;
        }
        text->setStr("NUMBER OF PLAYER : " + std::to_string(maxPlayerNB));
        if (isHost) {
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
        }
    }
}

void EventManager::handleGamemodeButton(ecs::Entity &entity, bool isHost)
{
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    size_t id = _game.getLocalPlayer()
                    .getComponent<ecs::PlayerComponent>()
                    ->getPlayerID();

    if (!text)
        return;
    if (isHost && id != _game.getHostClient())
        return;

    std::string str = text->getStr();
    std::size_t separator = str.find(":");
    std::stringstream ss;

    if (separator != std::string::npos) {
        std::string gameMode = str.substr(separator + 2);
        if (gameMode == "WAVE") {
            text->setStr("GAMEMODE : PVP");
            ss << Protocol::L_GAMEMODE << " " << id << " " << 1 << PACKET_END;
            _game.setGameMode(1);
        } else {
            text->setStr("GAMEMODE : WAVE");
            ss << Protocol::L_GAMEMODE << " " << id << " " << 0 << PACKET_END;
            _game.setGameMode(0);
        }
        if (isHost) {
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
        }
    }
}

void EventManager::mouseClicked()
{
    sf::Vector2f mousePos = _game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(_game.getWindow()));
    bool currentAutoFireValue = _game._gameConfig.getAutoFireConfig();
    for (size_t ent : _game.refEntityManager.getCurrentLevel()
                          .findEntitiesIdByComponent<ecs::RenderComponent>()) {
        auto &entity =
            _game.refEntityManager.getCurrentLevel().getEntityById(ent);
        auto button = entity.getComponent<ecs::RenderComponent>();
        if (!button
            || (button->getObjectType()
                    != ecs::RenderComponent::ObjectType::BUTTON
                && button->getObjectType()
                    != ecs::RenderComponent::ObjectType::INPUT))
            continue;

        auto rectangle =
            entity.getComponent<ecs::RectangleComponent<sf::RectangleShape>>();
        if (!rectangle)
            continue;

        //TO DO : Add sound effect
        //auto &buttonClikedSound = _game.refAssetManager.getAsset<sf::SoundBuffer>(Asset::BUTTON_CLICKED_SOUND);
        //_factory.buildSoundEffect(buttonClikedSound, "buttonClikedSound", 100.0f);

        sf::FloatRect currentHover =
            rectangle->getRectangle().getGlobalBounds();
        if (currentHover.contains(mousePos)) {
            auto clickableType =
                entity.getComponent<ecs::ClickableComponent>();

            auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
            if (!clickableType)
                continue;

            switch (clickableType->getClickableType()) {
                case ecs::ClickableType::MULTIPLAYER:
                    _game._buttonList.clear();
                    _game._inputList.clear();
                    _game.levelContinueMenu();
                    return;
                case ecs::ClickableType::LOBBY:
                    _game._buttonList.clear();
                    _game.connectToGame();
                    break;
                case ecs::ClickableType::EXIT:
                    _game.getWindow().close();
                    _game.refGameEngine.stop();
                    break;
                case ecs::ClickableType::SETTINGS:
                    _game._buttonList.clear();
                    _game._inputList.clear();
                    _game.levelSettingsMenu();
                    return;
                case ecs::ClickableType::AUTO_FIRE:
                    _game.handleAutoFireButton(
                        currentAutoFireValue ? "false" : "true", entity);
                    break;
                case ecs::ClickableType::BACK:
                    _game._buttonList.clear();
                    _game._inputList.clear();
                    _game.levelMainMenu();
                    return;
                case ecs::ClickableType::MOVE_UP:
                    _game._isSettingsUpButtonClicked = true;
                    _game._lastButtonIdClicked = entity.getID();
                    break;
                case ecs::ClickableType::MOVE_DOWN:
                    _game._isSettingsDownButtonClicked = true;
                    _game._lastButtonIdClicked = entity.getID();
                    break;
                case ecs::ClickableType::MOVE_LEFT:
                    _game._isSettingsLeftButtonClicked = true;
                    _game._lastButtonIdClicked = entity.getID();
                    break;
                case ecs::ClickableType::MOVE_RIGHT:
                    _game._isSettingsRightButtonClicked = true;
                    _game._lastButtonIdClicked = entity.getID();
                    break;
                case ecs::ClickableType::INPUT:
                    _game._isSettingsNicknameButtonClicked = true;
                    _game._lastInputIdClicked = entity.getID();
                    _game._nicknameKeys.clear();
                    break;
                case ecs::ClickableType::NUMBER_OF_PLAYER:
                    handleNumberOfPlayerButton(entity, true);
                    break;
                case ecs::ClickableType::DIFFICULTY:
                    handleDifficultyButton(entity, true);
                    break;
                case ecs::ClickableType::BONUS:
                    handleBonusButton(entity, true);
                    break;
                case ecs::ClickableType::LAUNCH:
                    handleStartButton(true);
                    break;
                case ecs::ClickableType::LEVEL:
                    handleLevelButton(entity, true);
                    break;
                case ecs::ClickableType::GAMEMODE:
                    handleGamemodeButton(entity, true);
                    break;
                default: break;
            }
        }
    }
}
