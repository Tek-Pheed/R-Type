/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** buildLobby.cpp
*/
#include <sstream>
#include "Game.hpp"
#include "GameAssets.hpp"
#include "Levels.hpp"

using namespace RType;

void Levels::buildLobby()
{
    if constexpr (!server) {
        std::string title = "GAME CONFIG";

        auto &text =
            _game.factory.buildText(0, 0, 0, title, sf::Color::White, 48);
        auto titleText = text.getComponent<ecs::TextComponent<sf::Text>>();
        auto titlePos = text.getComponent<ecs::PositionComponent>();

        titlePos->setX((float) (static_cast<float>(
                                    _game.getWindow().getSize().x - 350.0 / 2)
            - 50 - titleText->getText().getLocalBounds().width / 2));
        titlePos->setY((float) (this->_game.getWindow().getSize().y / 2.0
            - 50.0 / 2 - 75 * 1));

        _game.factory.buildButton(
            sf::Vector2f(
                (float) this->_game.getWindow().getSize().x - 350 - 50,
                (float) this->_game.getWindow().getSize().y / 2
                    - (float) 50 / 2 - (float) 75 * 1),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Black,
            "GAMEMODE : WAVE", 40, sf::Color::Black,
            ecs::ClickableType::GAMEMODE);

        std::stringstream ss;
        ss << _game.getMaxPlayers();
        _game.factory.buildButton(
            sf::Vector2f(
                (float) this->_game.getWindow().getSize().x - 350 - 50,
                (float) this->_game.getWindow().getSize().y / 2
                    - (float) 50 / 2 - (float) 75 * 0),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Black,
            "NUMBER OF PLAYER : " + ss.str(), 40, sf::Color::Black,
            ecs::ClickableType::NUMBER_OF_PLAYER);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) this->_game.getWindow().getSize().x - 350 - 50,
                (float) this->_game.getWindow().getSize().y / 2
                    - (float) 50 / 2 - (float) 75 * -1),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Black,
            "DIFFICULTY : EASY", 40, sf::Color::Black,
            ecs::ClickableType::DIFFICULTY);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) this->_game.getWindow().getSize().x - 350 - 50,
                (float) this->_game.getWindow().getSize().y / 2
                    - (float) 50 / 2 - (float) 75 * -2),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Black,
            "LEVEL : 1", 40, sf::Color::Black, ecs::ClickableType::LEVEL);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) this->_game.getWindow().getSize().x - 350 - 50,
                (float) this->_game.getWindow().getSize().y / 2
                    - (float) 50 / 2 - (float) 75 * -3),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Black,
            "BONUS : YES", 40, sf::Color::Black, ecs::ClickableType::BONUS);

        _game.factory.buildButton(
            sf::Vector2f(
                (float) this->_game.getWindow().getSize().x - 350 - 50,
                (float) this->_game.getWindow().getSize().y / 2
                    - (float) 50 / 2 - (float) 75 * -4),
            sf::Vector2f(350, 50), sf::Color::White, sf::Color::Black,
            "LAUNCH GAME", 40, sf::Color::Black, ecs::ClickableType::LAUNCH);
    }
}
