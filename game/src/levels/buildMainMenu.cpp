/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** buildMainMenu.cpp
*/

#include "Game.hpp"
#include "GameAssets.hpp"
#include "Levels.hpp"

static float randrange(float max, float min)
{
    return (min
        + static_cast<float>(rand())
            / (static_cast<float>((float) RAND_MAX / (float) (max - min))));
}

using namespace RType;

void Levels::buildMainMenu()
{
    _game.resetGame();
    _game.refEntityManager.deleteAllLevel();
    auto &level = _game.refEntityManager.createNewLevel("mainMenu");

    level.createSubsystem<GameSystems::RenderSystem>().initSystem(_game);
    level.createSubsystem<GameSystems::PositionSystem>().initSystem(_game);
    _game.refEntityManager.switchLevel("mainMenu");

    if constexpr (!server) {
        auto songEntity =
            _game.refEntityManager.getPersistentLevel()
                .findEntitiesByComponent<ecs::MusicComponent<sf::Sound>>()[0];
        auto currentSong =
            songEntity.get().getComponent<ecs::MusicComponent<sf::Sound>>();
        auto &musicSong =
            _game.refAssetManager.getAsset<sf::SoundBuffer>(Asset::MENU_SONG);

        currentSong->getMusicType().stop();
        currentSong->getMusicType().setBuffer(musicSong);
        currentSong->getMusicType().play();

        auto &title = _game.refEntityManager.getCurrentLevel().createEntity();
        sf::Text text;
        text.setFont(
            _game.refAssetManager.getAsset<sf::Font>(Asset::R_TYPE_FONT));
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.0f);
        text.setString("F-TYPE V8");
        float textWidth = text.getLocalBounds().width;
        float windowWidth = (float) _game.getWindow().getSize().x;
        float posX = (windowWidth - textWidth) / 2;
        float posY = (float) _game.getWindow().getSize().y / 10;

        auto comp = std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::TEXT);
        title.addComponent(comp);
        title.addComponent(
            std::make_shared<ecs::PositionComponent>(posX, posY));
        title.addComponent(
            std::make_shared<ecs::TextComponent<sf::Text>>(text, "F TypeV8"));

        _game.factory.buildAIPlayer(
            sf::Vector2f(randrange(0.4f, 0.1f), randrange(0.4f, 0.1f)),
            "Arnaud", static_cast<size_t>(rand() % 4));
        _game.factory.buildAIPlayer(
            sf::Vector2f(randrange(0.4f, 0.1f), randrange(0.4f, 0.1f)),
            "Lucas", static_cast<size_t>(rand() % 4));
        _game.factory.buildAIPlayer(
            sf::Vector2f(randrange(0.4f, 0.1f), randrange(0.4f, 0.1f)),
            "Raphael", static_cast<size_t>(rand() % 4));
        _game.factory.buildAIPlayer(
            sf::Vector2f(randrange(0.4f, 0.1f), randrange(0.4f, 0.1f)), "Samy",
            static_cast<size_t>(rand() % 4));
        _game.factory.buildAIPlayer(
            sf::Vector2f(randrange(0.4f, 0.1f), randrange(0.4f, 0.1f)),
            "Alexandre", static_cast<size_t>(rand() % 4));

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * 0),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "PLAY",
            40, sf::Color::Black, ecs::ClickableType::MULTIPLAYER);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -1),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black,
            "SETTINGS", 40, sf::Color::Black, ecs::ClickableType::SETTINGS);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) _game.getWindow().getSize().x / 2 - (float) 700 / 2,
                (float) _game.getWindow().getSize().y / 2 - (float) 50 / 2
                    - (float) 75 * -2),
            sf::Vector2f(700, 50), sf::Color::White, sf::Color::Black, "EXIT",
            40, sf::Color::Black, ecs::ClickableType::EXIT);

    }
}
