/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** getKeyFromString
*/

#include "Utils.hpp"

sf::Keyboard::Key Utils::getKeyFromString(const std::string &keyStr)
{
    static const std::pair<std::string, sf::Keyboard::Key> keyMap[] = {
        {"sf::Keyboard::A", sf::Keyboard::A}, {"sf::Keyboard::B", sf::Keyboard::B}, {"sf::Keyboard::C", sf::Keyboard::C}, {"sf::Keyboard::D", sf::Keyboard::D},
        {"sf::Keyboard::E", sf::Keyboard::E}, {"sf::Keyboard::F", sf::Keyboard::F}, {"sf::Keyboard::G", sf::Keyboard::G}, {"sf::Keyboard::H", sf::Keyboard::H},
        {"sf::Keyboard::I", sf::Keyboard::I}, {"sf::Keyboard::J", sf::Keyboard::J}, {"sf::Keyboard::K", sf::Keyboard::K}, {"sf::Keyboard::L", sf::Keyboard::L},
        {"sf::Keyboard::M", sf::Keyboard::M}, {"sf::Keyboard::N", sf::Keyboard::N}, {"sf::Keyboard::O", sf::Keyboard::O}, {"sf::Keyboard::P", sf::Keyboard::P},
        {"sf::Keyboard::Q", sf::Keyboard::Q}, {"sf::Keyboard::R", sf::Keyboard::R}, {"sf::Keyboard::S", sf::Keyboard::S}, {"sf::Keyboard::T", sf::Keyboard::T},
        {"sf::Keyboard::U", sf::Keyboard::U}, {"sf::Keyboard::V", sf::Keyboard::V}, {"sf::Keyboard::W", sf::Keyboard::W}, {"sf::Keyboard::X", sf::Keyboard::X},
        {"sf::Keyboard::Y", sf::Keyboard::Y}, {"sf::Keyboard::Z", sf::Keyboard::Z},
        {"sf::Keyboard::Num0", sf::Keyboard::Num0}, {"sf::Keyboard::Num1", sf::Keyboard::Num1}, {"sf::Keyboard::Num2", sf::Keyboard::Num2},
        {"sf::Keyboard::Num3", sf::Keyboard::Num3}, {"sf::Keyboard::Num4", sf::Keyboard::Num4}, {"sf::Keyboard::Num5", sf::Keyboard::Num5},
        {"sf::Keyboard::Num6", sf::Keyboard::Num6}, {"sf::Keyboard::Num7", sf::Keyboard::Num7}, {"sf::Keyboard::Num8", sf::Keyboard::Num8},
        {"sf::Keyboard::Num9", sf::Keyboard::Num9},
        {"sf::Keyboard::Up", sf::Keyboard::Up}, {"sf::Keyboard::Down", sf::Keyboard::Down},
        {"sf::Keyboard::Left", sf::Keyboard::Left}, {"sf::Keyboard::Right", sf::Keyboard::Right},
        {"sf::Keyboard::Escape", sf::Keyboard::Escape}, {"sf::Keyboard::Space", sf::Keyboard::Space},
        {"sf::Keyboard::Return", sf::Keyboard::Return}, {"sf::Keyboard::Tab", sf::Keyboard::Tab},
        {"sf::Keyboard::BackSpace", sf::Keyboard::BackSpace}, {"sf::Keyboard::Delete", sf::Keyboard::Delete},
        {"sf::Keyboard::Home", sf::Keyboard::Home}, {"sf::Keyboard::End", sf::Keyboard::End},
        {"sf::Keyboard::PageUp", sf::Keyboard::PageUp}, {"sf::Keyboard::PageDown", sf::Keyboard::PageDown}
    };

    for (const auto& pair : keyMap) {
        if (keyStr == pair.first) {
            return pair.second;
        }
    }

    return sf::Keyboard::Unknown;
}