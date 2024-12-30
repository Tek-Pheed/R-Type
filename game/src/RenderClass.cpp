/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "RenderClass.hpp"
#include <SFML/Graphics.hpp>
#include "ErrorClass.hpp"

RenderClass::RenderClass(
    int width, int height, const std::string &title, int frameRate)
{
    if (width <= 0 || height <= 0) {
        throw ErrorClass("RTC002 : Invalid window dimensions: width and "
                         "height must be positive.");
    }

    if (frameRate <= 0) {
        throw ErrorClass(
            "RTC003 : Invalid framerate: it must be a positive value.");
    }

    if (title.empty()) {
        throw ErrorClass(
            "RTC004 : Invalid title: the window title cannot be empty.");
    }

    this->_title = title;
    this->_videoMode =
        sf::VideoMode(1280, 720, sf::VideoMode::getDesktopMode().bitsPerPixel);
    this->_frameRate = frameRate;

    this->_window.create(this->_videoMode, title);
    this->_window.setFramerateLimit(static_cast<unsigned int>(frameRate));
    this->_bulletTexture.loadFromFile("./assets/sprites/r-typesheet1.gif");

    if (!this->_window.isOpen()) {
        throw ErrorClass("RTC001 : Failed to create the SFML window.");
    }
}

RenderClass::~RenderClass()
{
    if (this->_window.isOpen()) {
        this->_window.close();
    }
}

sf::RenderWindow &RenderClass::getWindow()
{
    return this->_window;
}

std::string RenderClass::getTitle() const
{
    return this->_title;
}

int RenderClass::getFrameRate() const
{
    return this->_frameRate;
}
void RenderClass::setTitle(const std::string &newTitle)
{
    if (newTitle.empty()) {
        throw ErrorClass(
            "RTC004 : Invalid title: the window title cannot be empty.");
    }
    this->_title = newTitle;
    this->_window.setTitle(newTitle);
}

void RenderClass::setFrameRate(int newFrameRate)
{
    if (newFrameRate <= 0) {
        throw ErrorClass(
            "RTC003 : Invalid framerate: it must be a positive value.");
    }
    this->_frameRate = newFrameRate;
    this->_window.setFramerateLimit(static_cast<unsigned int>(newFrameRate));
}

void RenderClass::setPlayerTexture(sf::Texture &texture)
{
    _playerTexture = texture;
}

sf::Texture &RenderClass::getPlayerTexture()
{
    return _playerTexture;
}

void RenderClass::setEnemyTexture(sf::Texture &texture)
{
    _enemyTexture = texture;
}

sf::Texture &RenderClass::getEnemyTexture()
{
    return _enemyTexture;
}

void RenderClass::setBulletTexture(sf::Texture &texture)
{
    _bulletTexture = texture;
}

sf::Texture &RenderClass::getBulletTexture()
{
    return _bulletTexture;
}

// void RenderClass::backgroundAnimation(sf::Sprite *bg, sf::Clock *clock)
// {
//     float s = clock->getElapsedTime().asSeconds();

//     if (s > 0.01) {
//         bg->setTextureRect(sf::Rect(bg->getTextureRect().left + 2, 0,
//             int(getWindow().getSize().x), int(getWindow().getSize().y)));
//         clock->restart();
//     }
// }