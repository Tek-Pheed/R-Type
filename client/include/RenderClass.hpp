/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Entity.hpp"
#include "client.hpp"

#ifndef RENDERCLASS_HPP_
    #define RENDERCLASS_HPP_

class RenderClass {
  public:
    RenderClass(
        int width, int height, const std::string &title, int frameRate);
    ~RenderClass();

    sf::RenderWindow &getWindow();
    std::string getTitle() const;
    void setTitle(const std::string &title);
    int getFrameRate() const;
    void setFrameRate(int frameRate);

    void renderWindow(std::shared_ptr<ecs::Entity> player, client &client);
    void playEvent(std::shared_ptr<ecs::Entity> player);
    void playerAnimations(
        std::shared_ptr<ecs::Entity> player, std::string direction);

    void setPlayerTexture(sf::Texture &texture);
    sf::Texture &getPlayerTexture();

  private:
    sf::RenderWindow _window;
    std::string _title;
    sf::VideoMode _videoMode;
    int _frameRate;
    sf::Texture _bulletTexture;
    sf::Texture _playerTexture;
};

void backgroundAnimation(sf::Sprite *bg, sf::Clock *clock);

#endif /* !RENDERCLASS_HPP_ */
