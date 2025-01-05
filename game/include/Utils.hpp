/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Utils
*/

#ifndef UTILSCLASS_HPP_
#define UTILSCLASS_HPP_

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Graphics.hpp>

class Utils {
public:
    Utils() = default;

    sf::Keyboard::Key getKeyFromString(const std::string &keyStr);
};

#endif /* !UTILSCLASS_HPP_ */
