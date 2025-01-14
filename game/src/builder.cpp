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
#include "Config.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameSystems.hpp"
#include "components/ClickableComponent.hpp"

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
        _factory.buildBackground();
    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::BackgroundSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::HealthSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::HitboxSystem>().initSystem(*this);
    auto &musicSong =
        this->refAssetManager.getAsset<sf::SoundBuffer>(Asset::MENU_SONG);
    _factory.buildMusic(musicSong, "menuSong");
}

ecs::Entity &RType::GameInstance::buildButton(std::string str, int buttonID)
{
    auto &button = refEntityManager.getCurrentLevel().createEntity();

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);
    rect.setSize(sf::Vector2f(700, 50));

    sf::Text text;
    text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Black);
    text.setString(str);

    button.addComponent(std::make_shared<ecs::PositionComponent>(
        (float) this->_window->getSize().x / 2 - (float) rect.getSize().x / 2,
        (float) this->_window->getSize().y / 2 - (float) rect.getSize().y / 2
            - (float) 75 * (float) buttonID));
    button.addComponent(
        std::make_shared<ecs::RectangleComponent<sf::RectangleShape>>(
            rect, rect.getSize().x, rect.getSize().y));
    button.addComponent(
        std::make_shared<ecs::TextComponent<sf::Text>>(text, str));
    button.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::BUTTON));
    button.addComponent(std::make_shared<ecs::ClickableComponent>(
        ecs::ClickableType::AUTO_FIRE));

    _buttonList.push_back(button);

    return button;
}

ecs::Entity &RType::GameInstance::buildInput(std::string str, int buttonID)
{
    auto &input = refEntityManager.getCurrentLevel().createEntity();

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);
    rect.setSize(sf::Vector2f(700, 50));

    sf::Text text;
    text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Black);
    text.setString(str);

    input.addComponent(std::make_shared<ecs::PositionComponent>(
        (float) this->_window->getSize().x / 2 - (float) rect.getSize().x / 2,
        (float) this->_window->getSize().y / 2 - (float) rect.getSize().y / 2
            - (float) 75 * (float) buttonID));
    input.addComponent(
        std::make_shared<ecs::RectangleComponent<sf::RectangleShape>>(
            rect, rect.getSize().x, rect.getSize().y));
    input.addComponent(
        std::make_shared<ecs::TextComponent<sf::Text>>(text, str));
    input.addComponent(std::make_shared<ecs::RenderComponent>(
        ecs::RenderComponent::ObjectType::INPUT));
    input.addComponent(
        std::make_shared<ecs::ClickableComponent>(ecs::ClickableType::INPUT));

    _inputList.push_back(input);

    return input;
}

void RType::GameInstance::levelMainMenu()
{
    auto &level = refEntityManager.createNewLevel("mainMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    refEntityManager.switchLevel("mainMenu");

    if (!isServer()) {
        auto songEntity =
            refEntityManager.getPersistentLevel()
                .findEntitiesByComponent<ecs::MusicComponent<sf::Sound>>()[0];
        auto currentSong =
            songEntity.get().getComponent<ecs::MusicComponent<sf::Sound>>();

        if (currentSong->getMusicType().getStatus()
            != sf::SoundSource::Playing) {
            currentSong->getMusicType().play();
        }

        auto &title = refEntityManager.getCurrentLevel().createEntity();
        sf::Text text;
        text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.0f);
        text.setString("F-TYPE V8");
        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float) this->_window->getSize().x;
        float posX = (windowWidth - textWidth) / 2;
        float posY = (float) this->_window->getSize().y / 10;

        auto comp = std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT);
        title.addComponent(comp);
        title.addComponent(
            std::make_shared<ecs::PositionComponent>(posX, posY));
        title.addComponent(
            std::make_shared<ecs::TextComponent<sf::Text>>(text, "F TypeV8"));

        _factory.buildAIPlayer(
            sf::Vector2f(static_cast<float>(rand() % 200 + 100),
                static_cast<float>(rand() % 200 + 100)),
            "Arnaud", static_cast<size_t>(rand() % 4));
        _factory.buildAIPlayer(
            sf::Vector2f(static_cast<float>(rand() % 200 + 100),
                static_cast<float>(rand() % 200 + 100)),
            "Lucas", static_cast<size_t>(rand() % 4));
        _factory.buildAIPlayer(
            sf::Vector2f(static_cast<float>(rand() % 200 + 100),
                static_cast<float>(rand() % 200 + 100)),
            "Raphael", static_cast<size_t>(rand() % 4));
        _factory.buildAIPlayer(
            sf::Vector2f(static_cast<float>(rand() % 200 + 100),
                static_cast<float>(rand() % 200 + 100)),
            "Samy", static_cast<size_t>(rand() % 4));
        _factory.buildAIPlayer(
            sf::Vector2f(static_cast<float>(rand() % 200 + 100),
                static_cast<float>(rand() % 200 + 100)),
            "Alexandre", static_cast<size_t>(rand() % 4));

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 0),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "PLAY",
            40, sf::Color::Black, ecs::ClickableType::MULTIPLAYER);

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -1),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            "SETTINGS", 40, sf::Color::Black, ecs::ClickableType::SETTINGS);

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -2),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "EXIT",
            40, sf::Color::Black, ecs::ClickableType::EXIT);
    }
}

void RType::GameInstance::levelLobbyMenu()
{
    if (!isServer()) {
        std::string title = "GAME CONFIG";

        auto &text = _factory.buildText(0, 0, 0, title);
        auto titleText = text.getComponent<ecs::TextComponent<sf::Text>>();
        auto titlePos = text.getComponent<ecs::PositionComponent>();

        titlePos->setX(
            (float) (static_cast<float>(_window->getSize().x - 350 / 2) - 50
                - titleText->getText().getLocalBounds().width / 2));
        titlePos->setY(
            (float) (this->_window->getSize().y / 2 - 50 / 2 - 75 * 1));

        _factory.buildButton(
            sf::Vector2f((float) this->_window->getSize().x - 350 - 50,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 1),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Blue,
            "GAMEMODE : WAVE", 40, sf::Color::Black,
            ecs::ClickableType::GAMEMODE);

        _factory.buildButton(
            sf::Vector2f((float) this->_window->getSize().x - 350 - 50,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 0),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Blue,
            "NUMBER OF PLAYER : 4", 40, sf::Color::Black,
            ecs::ClickableType::NUMBER_OF_PLAYER);

        _factory.buildButton(
            sf::Vector2f((float) this->_window->getSize().x - 350 - 50,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -1),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Blue,
            "DIFFICULTY : EASY", 40, sf::Color::Black,
            ecs::ClickableType::DIFFICULTY);

        _factory.buildButton(
            sf::Vector2f((float) this->_window->getSize().x - 350 - 50,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -2),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Blue,
            "LEVEL : 1", 40, sf::Color::Black, ecs::ClickableType::LEVEL);

        _factory.buildButton(
            sf::Vector2f((float) this->_window->getSize().x - 350 - 50,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -3),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Blue,
            "BONUS : YES", 40, sf::Color::Black, ecs::ClickableType::BONUS);

        _factory.buildButton(
            sf::Vector2f((float) this->_window->getSize().x - 350 - 50,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -4),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Blue,
            "LAUNCH GAME", 40, sf::Color::Black, ecs::ClickableType::LAUNCH);
    }
}

void RType::GameInstance::levelContinueMenu()
{
    auto &level = refEntityManager.createNewLevel("continueMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(*this);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(*this);
    refEntityManager.switchLevel("continueMenu");

    if (!isServer()) {
        auto &title = refEntityManager.getCurrentLevel().createEntity();

        sf::Text text;
        text.setFont(refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.0f);
        text.setString("CONNECTION TO SERVER");

        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float) this->_window->getSize().x;

        float posX = (windowWidth - textWidth) / 2;
        float posY = (float) this->_window->getSize().y / 10;

        title.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT));
        title.addComponent(
            std::make_shared<ecs::PositionComponent>(posX, posY));
        title.addComponent(std::make_shared<ecs::TextComponent<sf::Text>>(
            text, "CONNECTION TO SERVER"));

        buildInput("NICKNAME", 2);
        buildInput("IP ADRESS", 1);
        buildInput("8081", 0);
        buildInput("8082", -1);
        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -3),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Blue,
            "PLAY GAME", 40, sf::Color::Black, ecs::ClickableType::LOBBY);
        // sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
        //"PLAY GAME", 40, sf::Color::Black, ecs::ClickableType::LAUNCH);
        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -4),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "BACK",
            40, sf::Color::Black, ecs::ClickableType::BACK);
    }
}

void RType::GameInstance::levelSettingsMenu()
{
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
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.0f);
        text.setString("SETTINGS");

        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float) this->_window->getSize().x;

        float posX = (windowWidth - textWidth) / 2;
        float posY = (float) this->_window->getSize().y / 10;

        title.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT));
        title.addComponent(
            std::make_shared<ecs::PositionComponent>(posX, posY - 50));
        title.addComponent(
            std::make_shared<ecs::TextComponent<sf::Text>>(text, "SETTINGS"));

        std::string moveUpAction = _gameConfig.getConfig().at(1);
        std::string moveRightAction = _gameConfig.getConfig().at(2);
        std::string moveLeftAction = _gameConfig.getConfig().at(3);
        std::string moveDownAction = _gameConfig.getConfig().at(4);
        std::string autoFireAction = _gameConfig.getConfig().at(7);

        std::string endUpMove = "";
        std::string endRightMove = "";
        std::string endLeftMove = "";
        std::string endDownMove = "";
        std::string endAutoFire = "";

        size_t equalPosUp = moveUpAction.find("=");
        if (equalPosUp != std::string::npos) {
            std::string beforeUpEqual = "MOVE UP";
            std::string afterUpEqual = moveUpAction.substr(equalPosUp + 1);
            size_t firstdoublepoint = afterUpEqual.find("::");
            std::string afterfirstdoublepoint =
                afterUpEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endUpMove = beforeUpEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosRight = moveRightAction.find("=");
        if (equalPosRight != std::string::npos) {
            std::string beforeRightEqual = "MOVE RIGHT";
            std::string afterRightEqual =
                moveRightAction.substr(equalPosRight + 1);
            size_t firstdoublepoint = afterRightEqual.find("::");
            std::string afterfirstdoublepoint =
                afterRightEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endRightMove = beforeRightEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosLeft = moveLeftAction.find("=");
        if (equalPosLeft != std::string::npos) {
            std::string beforeLeftEqual = "MOVE LEFT";
            std::string afterLeftEqual =
                moveLeftAction.substr(equalPosLeft + 1);
            size_t firstdoublepoint = afterLeftEqual.find("::");
            std::string afterfirstdoublepoint =
                afterLeftEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endLeftMove = beforeLeftEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosDown = moveDownAction.find("=");
        if (equalPosDown != std::string::npos) {
            std::string beforeDownEqual = "MOVE DOWN";
            std::string afterDownEqual =
                moveDownAction.substr(equalPosDown + 1);
            size_t firstdoublepoint = afterDownEqual.find("::");
            std::string afterfirstdoublepoint =
                afterDownEqual.substr(firstdoublepoint + 2);
            size_t seconddoublepoint = afterfirstdoublepoint.find("::");
            std::string aftersedoublepoint =
                afterfirstdoublepoint.substr(seconddoublepoint + 2);
            endDownMove = beforeDownEqual + " : " + aftersedoublepoint;
        }

        size_t equalPosAutoFire = autoFireAction.find("=");
        if (equalPosAutoFire != std::string::npos) {
            std::string beforeAutoFireEqual = "AUTO FIRE";
            std::string afterAutoFireEqual =
                autoFireAction.substr(equalPosAutoFire + 1);
            endAutoFire = beforeAutoFireEqual + " : "
                + (afterAutoFireEqual == "true" ? "Yes" : "No");
        }

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 2),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endUpMove, 40, sf::Color::Black, ecs::ClickableType::MOVE_UP);

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 1),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endDownMove, 40, sf::Color::Black, ecs::ClickableType::MOVE_DOWN);

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 0),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endRightMove, 40, sf::Color::Black,
            ecs::ClickableType::MOVE_RIGHT);

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -1),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endLeftMove, 40, sf::Color::Black, ecs::ClickableType::MOVE_LEFT);

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -2),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            endAutoFire, 40, sf::Color::Black, ecs::ClickableType::AUTO_FIRE);

        _factory.buildButton(
            sf::Vector2f(
                (float) this->_window->getSize().x / 2 - (float) 700 / 2,
                (float) this->_window->getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -3),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "BACK",
            40, sf::Color::Black, ecs::ClickableType::BACK);
    }
}

void RType::GameInstance::handleAutoFireButton(
    std::string newAutoFireValue, ecs::Entity &entity)
{
    std::string value = (newAutoFireValue == "true" ? "YES" : "NO");
    _gameConfig.updateConfigValue("AUTO_FIRE=", newAutoFireValue);
    _gameConfig.saveConfig();
    auto text = entity.getComponent<ecs::TextComponent<sf::Text>>();
    text->setStr("AUTO FIRE : " + value);
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

        if (text && entity.get().getID() == _lastInputIdClicked) {
            if (nickname.empty()) {
                text->setStr("");
            } else {
                text->setStr(nickname);
            }
            break;
        }
    }
}

void RType::GameInstance::levelPauseMenu()
{
    
}

void RType::GameInstance::handleConfigButtons(
    sf::Keyboard::Key pressedKey, int actionType)
{
    if (!isServer()) {
        std::string pressedKeyString = getKeyString(pressedKey);
        std::string newValue = "sf::Keyboard::" + pressedKeyString;

        switch (actionType) {
            case 0: {
                std::string moveUpAction = _gameConfig.getConfig().at(1);
                size_t equalPosUp = moveUpAction.find("=");
                if (equalPosUp != std::string::npos) {
                    moveUpAction = moveUpAction.substr(equalPosUp + 1);
                }
                _gameConfig.updateConfigValue("MOVE_UP=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == _lastButtonIdClicked) {
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
                std::string moveRightAction = _gameConfig.getConfig().at(2);
                size_t equalPosRight = moveRightAction.find("=");
                if (equalPosRight != std::string::npos) {
                    moveRightAction =
                        moveRightAction.substr(equalPosRight + 1);
                }
                _gameConfig.updateConfigValue("MOVE_RIGHT=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == _lastButtonIdClicked) {
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
                std::string moveLeftAction = _gameConfig.getConfig().at(3);
                size_t equalPosLeft = moveLeftAction.find("=");
                if (equalPosLeft != std::string::npos) {
                    moveLeftAction = moveLeftAction.substr(equalPosLeft + 1);
                }
                _gameConfig.updateConfigValue("MOVE_LEFT=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == _lastButtonIdClicked) {
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
                std::string moveDownAction = _gameConfig.getConfig().at(4);
                size_t equalPosDown = moveDownAction.find("=");
                if (equalPosDown != std::string::npos) {
                    moveDownAction = moveDownAction.substr(equalPosDown + 1);
                }
                _gameConfig.updateConfigValue("MOVE_DOWN=", newValue);
                for (auto &entity :
                    refEntityManager.getCurrentLevel().getEntities()) {
                    if (entity.get().getID() == _lastButtonIdClicked) {
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
        _gameConfig.saveConfig();
    }
}
