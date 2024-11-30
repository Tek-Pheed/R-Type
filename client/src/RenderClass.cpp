/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

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

void RenderClass::renderWindow()
{
    while (this->_window.isOpen()) {
        this->_window.clear(sf::Color::Black);
        playEvent();
        this->_window.display();
    }
}

void RenderClass::playEvent()
{
    sf::Event event;

    while (this->_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window.close();
        }
    }
}
