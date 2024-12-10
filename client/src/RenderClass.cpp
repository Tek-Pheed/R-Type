/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#include "RenderClass.hpp"
#include <SFML/Graphics.hpp>
#include "ErrorClass.hpp"
#include "Systems.hpp"
#include "client.hpp"

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

void RenderClass::renderWindow(
    std::shared_ptr<ecs::Entity> player, client &client)
{
    sf::Clock clock;
    sf::Clock clockAnim;
    ecs::RenderSystem renderSystem;
    ecs::PositionSystem positionSystem;
    sf::Texture background_t;
    sf::Sprite background_s;
    float deltaTime = 0.00;

    _window.setMouseCursorVisible(false);
    background_t.loadFromFile("./assets/background/starry_night.png");
    background_s.setTextureRect(sf::Rect(0, 0, 1280, 720));
    background_s.setTexture(background_t);

    while (this->_window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        this->_window.clear();
        playEvent(player);
        this->_window.draw(background_s);
        positionSystem.update(
            client.get_entities(), &this->_window, deltaTime);
        client.manage_buffers();
        renderSystem.update(client.get_entities(), &this->_window, deltaTime);
        this->_window.display();
        backgroundAnimation(&background_s, &clockAnim);
    }
}

void RenderClass::playEvent(std::shared_ptr<ecs::Entity> player)
{
    sf::Event event;
    auto velocity = player->getComponent<ecs::VelocityComponent>();

    while (this->_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->_window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                velocity->setVy(-200.0f);
                this->playerAnimations(player, "top");
            } else if (event.key.code == sf::Keyboard::Down) {
                velocity->setVy(200.0f);
                this->playerAnimations(player, "down");
            } else if (event.key.code == sf::Keyboard::Right) {
                velocity->setVx(200.0f);
                this->playerAnimations(player, "right");
            } else if (event.key.code == sf::Keyboard::Left) {
                velocity->setVx(-200.0f);
                this->playerAnimations(player, "left");
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            this->playerAnimations(player, "none");
            if (event.key.code == sf::Keyboard::Up
                || event.key.code == sf::Keyboard::Down) {
                velocity->setVy(0.0f);
            }
            if (event.key.code == sf::Keyboard::Left
                || event.key.code == sf::Keyboard::Right) {
                velocity->setVx(0.0f);
            }
        }
    }
}

void RenderClass::playerAnimations(
    std::shared_ptr<ecs::Entity> player, std::string direction)
{
    if (direction == "top") {
        player->getComponent<ecs::RenderComponent>()
            ->getSprite()
            ->setTextureRect(sf::Rect(132, 0, 33, 14));
    } else if (direction == "down") {
        player->getComponent<ecs::RenderComponent>()
            ->getSprite()
            ->setTextureRect(sf::Rect(0, 0, 33, 14));
    } else {
        player->getComponent<ecs::RenderComponent>()
            ->getSprite()
            ->setTextureRect(sf::Rect(66, 0, 33, 14));
    }
}

void backgroundAnimation(sf::Sprite *bg, sf::Clock *clock)
{
    float s = clock->getElapsedTime().asSeconds();

    if (s > 0.01) {
        bg->setTextureRect(
            sf::Rect(bg->getTextureRect().left + 2, 0, 1280, 720));
        if (bg->getTextureRect().left > 768) {
            bg->setTextureRect(sf::Rect(0, 0, 1280, 720));
        }
        clock->restart();
    }
}
