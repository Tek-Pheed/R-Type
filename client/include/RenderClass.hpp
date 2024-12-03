/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** RenderClass
*/

#include <SFML/Graphics.hpp>
#include <string>

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

    void renderWindow();
    void playEvent();

  private:
    sf::RenderWindow _window;
    std::string _title;
    sf::VideoMode _videoMode;
    int _frameRate;
};

#endif /* !RENDERCLASS_HPP_ */
