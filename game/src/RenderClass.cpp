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
#include <sstream>
#include "Components.hpp"
#include "ErrorClass.hpp"
#include "GameSystems.hpp"
#include "game.hpp"

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

// void RenderClass::renderWindow(Game &client)
// {
//     sf::Clock clock;
//     sf::Clock clockAnim;
//     GameSystems::RenderSystem renderSystem;
//     GameSystems::PositionSystem positionSystem;
//     GameSystems::BulletSystem bulletSystem;
//     sf::Texture background_t;
//     sf::Sprite background_s;
//     auto player = client.getLocalPlayer();
//     float deltaTime = 0.00;

//     _window.setMouseCursorVisible(false);
//     background_t.loadFromFile("assets/background/background.png");
//     background_t.setRepeated(true);
//     background_s.setTextureRect(sf::Rect(0, 0, 1280, 720));
//     background_s.setTexture(background_t);

//     while (this->_window.isOpen()) {
//         deltaTime = clock.restart().asSeconds();
//         this->_window.clear();
//         playEvent(client, client.getEntities());
//         this->_window.draw(background_s);
//         positionSystem.update(client.getEntities(), deltaTime);
//         client.updateLocalPlayerPosition();
//         renderSystem.update(client.getEntities(), deltaTime);
//         client.manageBuffers();
//         bulletSystem.update(client.getEntities(), deltaTime);
//         this->_window.display();
//         backgroundAnimation(&background_s, &clockAnim);
//     }
// }

// void RenderClass::playEvent(Game &client, std::vector<ecs::Entity> &entities)
// {
//     sf::Event event;
//     std::stringstream ss;
//     auto &player = client.getLocalPlayer();
//     auto velocity = player.getComponent<ecs::VelocityComponent>();

//     while (this->_window.pollEvent(event)) {
//         if (event.type == sf::Event::Closed) {
//             this->_window.close();
//         }
//         if (event.type == sf::Event::KeyPressed) {
//             if (event.key.code == sf::Keyboard::Up) {
//                 velocity->setVy(-200.0f);
//                 this->playerAnimations(player, "top");
//             } else if (event.key.code == sf::Keyboard::Down) {
//                 velocity->setVy(200.0f);
//                 this->playerAnimations(player, "down");
//             } else if (event.key.code == sf::Keyboard::Right) {
//                 velocity->setVx(200.0f);
//                 this->playerAnimations(player, "right");
//             } else if (event.key.code == sf::Keyboard::Left) {
//                 velocity->setVx(-200.0f);
//                 this->playerAnimations(player, "left");
//             } else if (event.key.code == sf::Keyboard::Space) {
//                 auto bullet = ecs::Entity((std::size_t) rand());
//                 bullet.addComponent(std::make_shared<ecs::BulletComponent>(1));
//                 bullet.addComponent(std::make_shared<ecs::PositionComponent>(
//                     player.getComponent<ecs::PositionComponent>()->getX()
//                         + 100,
//                     player.getComponent<ecs::PositionComponent>()->getY()
//                         + 25));
//                 bullet.addComponent(
//                     std::make_shared<ecs::VelocityComponent>(350.0f, 0));
//                 sf::Sprite sprite(this->_bulletTexture);
//                 sprite.setTextureRect(sf::Rect(137, 153, 64, 16));
//                 bullet.addComponent(std::make_shared<ecs::RenderComponent>(
//                     ecs::RenderComponent::ObjectType::SPRITE));
//                 bullet.addComponent(
//                     std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
//                         sprite, 0, 0));
//                 ss << "104 " << client.getPlayerId() << "\t\n";
//                 // client.writeToServer(ss.str(),
//                 // System::Network::ISocket::UDP);
//                 entities.push_back(bullet);
//             }
//         }

//         if (event.type == sf::Event::KeyReleased) {
//             this->playerAnimations(player, "none");
//             if (event.key.code == sf::Keyboard::Up
//                 || event.key.code == sf::Keyboard::Down) {
//                 velocity->setVy(0.0f);
//             }
//             if (event.key.code == sf::Keyboard::Left
//                 || event.key.code == sf::Keyboard::Right) {
//                 velocity->setVx(0.0f);
//             }
//         }
//     }
// }

// void RenderClass::playerAnimations(ecs::Entity &player, std::string direction)
// {
//     if (direction == "top") {
//         player.getComponent<ecs::SpriteComponent<sf::Sprite>>()
//             ->getSprite()
//             .setTextureRect(sf::Rect(132, 0, 33, 14));
//     } else if (direction == "down") {
//         player.getComponent<ecs::SpriteComponent<sf::Sprite>>()
//             ->getSprite()
//             .setTextureRect(sf::Rect(0, 0, 33, 14));
//     } else {
//         player.getComponent<ecs::SpriteComponent<sf::Sprite>>()
//             ->getSprite()
//             .setTextureRect(sf::Rect(66, 0, 33, 14));
//     }
// }

// void RenderClass::backgroundAnimation(sf::Sprite *bg, sf::Clock *clock)
// {
//     float s = clock->getElapsedTime().asSeconds();

//     if (s > 0.01) {
//         bg->setTextureRect(sf::Rect(bg->getTextureRect().left + 2, 0,
//             int(getWindow().getSize().x), int(getWindow().getSize().y)));
//         clock->restart();
//     }
// }