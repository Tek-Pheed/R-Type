/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#include "../include/RenderClass.hpp"

RenderClass::RenderClass(unsigned int width, unsigned int height, const std::string &title, unsigned int frameRate)
    : videoMode(width, height), title(title), frameRate(frameRate)
{
    window.create(videoMode, title);
    window.setFramerateLimit(frameRate);
}

RenderClass::~RenderClass()
{
    if (window.isOpen()) {
        window.close();
    }
}

std::string RenderClass::getTitle() const
{
    return this->title;
}

unsigned int RenderClass::getFrameRate() const
{
    return this->frameRate;
}

void RenderClass::setTitle(const std::string &newTitle)
{
    this->title = newTitle;
    window.setTitle(newTitle);
}

void RenderClass::setFrameRate(unsigned int newFrameRate)
{
    this->frameRate = newFrameRate;
    window.setFramerateLimit(newFrameRate);
}

void RenderClass::renderWindow()
{
    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        playEvent();
        window.display();
    }
}

void RenderClass::playEvent()
{
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}
