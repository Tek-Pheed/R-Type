/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

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

    void renderWindow(std::vector<std::shared_ptr<ecs::Entity>> entities, std::shared_ptr<ecs::Entity> player);
    void playEvent(std::shared_ptr<ecs::Entity> player);
    void playerAnimations(std::shared_ptr<ecs::Entity> player, std::string direction);

  private:
    sf::RenderWindow _window;
    std::string _title;
    sf::VideoMode _videoMode;
    int _frameRate;
};

void backgroundAnimation(sf::Sprite *bg, sf::Clock *clock);

#endif /* !RENDERCLASS_HPP_ */
