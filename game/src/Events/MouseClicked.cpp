/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MouseClicked.cpp
*/

#include "SFML/Graphics.hpp"
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include "Components.hpp"
#include "Config.hpp"
#include "Entity.hpp"
#include "Events.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "components/ClickableComponent.hpp"

using namespace RType;

void EventManager::handleStartButton(bool isHost)
{
    if constexpr (server)
        return;
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
    if constexpr (server)
        return;
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
    if constexpr (server)
        return;
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
    if constexpr (server)
        return;
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

        if (levelNB >= _game.nbTxtFiles) {
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
    if constexpr (server)
        return;
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
    if constexpr (server)
        return;
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
    if constexpr (server)
        return;
    sf::Vector2f mousePos = _game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(_game.getWindow()));
    bool currentAutoFireValue = _game.gameConfig.getAutoFireConfig();
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

        auto &buttonClikedSound =
            _game.refAssetManager.getAsset<sf::SoundBuffer>(
                Asset::BUTTON_CLICKED_SOUND);

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
                    _game.buttonList.clear();
                    _game.inputList.clear();
                    _game.levels.buildConnectToServerMenu();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    return;
                case ecs::ClickableType::LOBBY:
                    _game.buttonList.clear();
                    _game.connectToGame();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::EXIT:
                    _game.getWindow().close();
                    _game.refGameEngine.stop();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::SETTINGS:
                    _game.buttonList.clear();
                    _game.inputList.clear();
                    _game.levels.buildSettingsMenu();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    return;
                case ecs::ClickableType::AUTO_FIRE:
                    _game.handleAutoFireButton(
                        currentAutoFireValue ? "false" : "true", entity);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::RESOLUTION:
                    _game.handleResolutionButton(entity);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::BACK:
                    _game.buttonList.clear();
                    _game.inputList.clear();
                    _game.levels.buildMainMenu();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    return;
                case ecs::ClickableType::MOVE_UP:
                    _game.isSettingsUpButtonClicked = true;
                    _game.lastButtonIdClicked = entity.getID();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::MOVE_DOWN:
                    _game.isSettingsDownButtonClicked = true;
                    _game.lastButtonIdClicked = entity.getID();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::MOVE_LEFT:
                    _game.isSettingsLeftButtonClicked = true;
                    _game.lastButtonIdClicked = entity.getID();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::MOVE_RIGHT:
                    _game.isSettingsRightButtonClicked = true;
                    _game.lastButtonIdClicked = entity.getID();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::INPUT:
                    _game.isSettingsNicknameButtonClicked = true;
                    _game.lastInputIdClicked = entity.getID();
                    _game.nicknameKeys.clear();
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::NUMBER_OF_PLAYER:
                    handleNumberOfPlayerButton(entity, true);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::DIFFICULTY:
                    handleDifficultyButton(entity, true);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::BONUS:
                    handleBonusButton(entity, true);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::LAUNCH:
                    handleStartButton(true);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::LEVEL:
                    handleLevelButton(entity, true);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                case ecs::ClickableType::GAMEMODE:
                    handleGamemodeButton(entity, true);
                    _game.factory.buildSoundEffect(
                        buttonClikedSound, "buttonClikedSound", 100.0f);
                    break;
                default: break;
            }
        }
    }
}
