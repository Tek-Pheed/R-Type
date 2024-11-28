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

        RenderClass(unsigned int width, unsigned int height, const std::string &title, unsigned int frameRate);
        ~RenderClass();

        std::string getTitle() const;
        void setTitle(const std::string &title);
        unsigned int getFrameRate() const;
        void setFrameRate(unsigned int frameRate);

        void renderWindow();
        void playEvent();

    private:

        sf::RenderWindow window;
        std::string title;
        sf::VideoMode videoMode;
        unsigned int frameRate;
};

#endif /* !RENDERCLASS_HPP_ */
