/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#ifndef RENDERCLASS_HPP_
#define RENDERCLASS_HPP_

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.hpp"

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

    // void renderWindow(Game &client);
    // void playEvent(Game &client, std::vector<ecs::Entity> &entities);
    void playerAnimations(ecs::Entity &player, std::string direction);
    void backgroundAnimation(sf::Sprite *bg, sf::Clock *clock);

    void setPlayerTexture(sf::Texture &texture);
    sf::Texture &getPlayerTexture();

    void setBulletTexture(sf::Texture &texture);
    sf::Texture &getBulletTexture();

    void setEnemyTexture(sf::Texture &texture);
    sf::Texture &getEnemyTexture();

  private:
    sf::RenderWindow _window;
    std::string _title;
    sf::VideoMode _videoMode;
    int _frameRate;
    sf::Texture _bulletTexture;
    sf::Texture _playerTexture;
    sf::Texture _enemyTexture;
};

#endif /* !RENDERCLASS_HPP_ */