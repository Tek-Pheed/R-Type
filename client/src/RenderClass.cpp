/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#include "../include/RenderClass.hpp"
#include "../include/ErrorClass.hpp"
#include <iostream>

RenderClass::RenderClass(int width, int height, const std::string &title, int frameRate)
    : videoMode(width, height), title(title), frameRate(frameRate)
{
    if (width <= 0 || height <= 0) {
        throw ErrorClass("RTC002 : Invalid window dimensions: width and height must be positive.");
    }

    if (frameRate <= 0) {
        throw ErrorClass("RTC003 : Invalid framerate: it must be a positive value.");
    }

    if (title.empty()) {
        throw ErrorClass("RTC004 : Invalid title: the window title cannot be empty.");
    }

    window.create(videoMode, title);
    window.setFramerateLimit(frameRate);

    if (!window.isOpen()) {
        throw ErrorClass("RTC001 : Failed to create the SFML window.");
    }
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

int RenderClass::getFrameRate() const
{
    return this->frameRate;
}

void RenderClass::setTitle(const std::string &newTitle)
{
    if (newTitle.empty()) {
        throw ErrorClass("RTC004 : Invalid title: the window title cannot be empty.");
    }
    this->title = newTitle;
    window.setTitle(newTitle);
}

void RenderClass::setFrameRate(int newFrameRate)
{
    if (newFrameRate <= 0) {
        throw ErrorClass("RTC003 : Invalid framerate: it must be a positive value.");
    }
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
